/*
 * Acquisition MPU-6050 sur ESP32-C3 — Projet 5 : analyse des mouvements
 *
 * Câblage :  MPU SDA -> GPIO 5 | MPU SCL -> GPIO 6 | VCC -> 3V3 | GND -> GND
 *
 * Acquisition :
 *   - Fréquence d'échantillonnage FIXE : Fe = 100 Hz (période 10 ms),
 *     cadencée par micros() (et non par delay(), qui dérive).
 *   - Plage ±2 g (16384 LSB/g) : meilleure résolution pour des mouvements lents.
 *   - Filtre passe-bas interne (DLPF) à ~44 Hz : filtre anti-repliement
 *     placé sous la fréquence de Nyquist (Fe/2 = 50 Hz).
 *
 * Sortie série (format CSV, une ligne par échantillon) :
 *   t_us,ax_g,ay_g,az_g
 *   t_us     : instant de la mesure en microsecondes depuis le démarrage de la carte
 *   ax/ay/az : accélération en g (5 décimales ; 1 LSB = 1/16384 g ≈ 0,00006 g)
 *   Les lignes commençant par '#' sont des messages d'information (ignorés par Python).
 *
 * La norme |a| n'est PAS calculée ici : elle sera calculée en Python,
 * afin que le CSV ne contienne que les données brutes du capteur.
 */

#include <Wire.h>

// ---------- Broches et adresse I2C ----------
#define SDA_PIN  5
#define SCL_PIN  6
#define MPU_ADDR 0x68      // Adresse par défaut (broche AD0 à la masse / non connectée).
                           // Si AD0 est relié à 3,3 V, l'adresse devient 0x69.

// ---------- Registres internes du MPU-6050 (datasheet / register map) ----------
#define REG_CONFIG       0x1A  // Configuration du filtre passe-bas numérique (DLPF)
#define REG_ACCEL_CONFIG 0x1C  // Plage de mesure de l'accéléromètre
#define REG_ACCEL_XOUT_H 0x3B  // Premier des 6 octets : X_H, X_L, Y_H, Y_L, Z_H, Z_L
#define REG_PWR_MGMT_1   0x6B  // Gestion d'alimentation : le capteur démarre en veille
#define REG_WHO_AM_I     0x75  // Registre d'identité : renvoie 0x68 sur un composant d'origine

// ---------- Plage de mesure ----------
// Le capteur fournit des entiers 16 bits. La plage fixe combien d'unités brutes = 1 g :
//   ±2g  -> 16384 LSB/g  <- choisi : mouvements lents, résolution maximale
//   ±4g  ->  8192 LSB/g
//   ±8g  ->  4096 LSB/g
//   ±16g ->  2048 LSB/g
#define ACCEL_RANGE_BITS 0x00   // ±2g (bits 4-3 = 00)
const float LSB_PER_G = 16384.0;

// ---------- Filtre passe-bas interne (DLPF_CFG, bits 2-0 du registre 0x1A) ----------
// Bande passante de l'accéléromètre selon DLPF_CFG :
//   0 -> 260 Hz | 1 -> 184 Hz | 2 -> 94 Hz | 3 -> 44 Hz | 4 -> 21 Hz | 5 -> 10 Hz | 6 -> 5 Hz
// Choix : 3 (44 Hz) < Fe/2 = 50 Hz  => limite le repliement spectral.
#define DLPF_CFG 0x03

// ---------- Fréquence d'échantillonnage ----------
const uint32_t FS_HZ     = 100;             // Fe = 100 Hz
const uint32_t PERIOD_US = 1000000UL / FS_HZ; // Te = 10 000 µs
uint32_t nextSampleUs;                        // instant prévu du prochain échantillon

// Écrit un octet dans un registre du MPU.
void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);     // quel registre
  Wire.write(value);   // quelle valeur
  Wire.endTransmission();
}

// Lit un octet dans un registre du MPU.
uint8_t readRegister(uint8_t reg) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);           // false = bus maintenu (repeated start)
  Wire.requestFrom(MPU_ADDR, (uint8_t)1);
  return Wire.read();
}

void setup() {
  Serial.begin(115200);
  delay(1000);                           // laisse le temps à la liaison USB de s'établir

  Wire.begin(SDA_PIN, SCL_PIN);          // I2C sur nos broches
  Wire.setClock(400000);                 // mode "fast" 400 kHz, supporté par le MPU

  // 1) Vérifie que le capteur répond
  Wire.beginTransmission(MPU_ADDR);
  if (Wire.endTransmission() != 0) {
    Serial.println("# ERREUR : aucun capteur a l'adresse I2C. Verifier cablage/soudures.");
    while (true) delay(1000);            // arrêt
  }

  // 2) Identité (les clones peuvent répondre 0x70, 0x72 ou 0x98 — généralement fonctionnels)
  uint8_t id = readRegister(REG_WHO_AM_I);
  Serial.print("# WHO_AM_I = 0x");
  Serial.println(id, HEX);

  // 3) Réveil du capteur (écrire 0 efface le bit SLEEP)
  writeRegister(REG_PWR_MGMT_1, 0x00);
  delay(100);

  // 4) Filtre passe-bas interne et plage de mesure
  writeRegister(REG_CONFIG, DLPF_CFG);
  writeRegister(REG_ACCEL_CONFIG, ACCEL_RANGE_BITS);

  Serial.println("# MPU-6050 pret. Fe = 100 Hz, plage = +/-2 g, DLPF = 44 Hz");
  Serial.println("t_us,ax_g,ay_g,az_g");

  nextSampleUs = micros();
}

void loop() {
  // Attente active jusqu'à l'instant prévu : cadence fixe sans dérive cumulée.
  // (la soustraction en entiers non signés gère le débordement de micros() après ~71 min)
  uint32_t t = micros();                 // horodatage réel de la mesure
  if ((int32_t)(t - nextSampleUs) < 0) return;
  nextSampleUs += PERIOD_US;             // le suivant est planifié depuis l'instant PRÉVU

  // Si on a pris plus d'une période de retard (ex. : envoi série bloqué tant qu'aucun
  // programme n'a ouvert le port), on se RECALE au lieu de rattraper le retard en rafale
  // (une rafale enverrait des échantillons à ~1,6 kHz au lieu de 100 Hz).
  // Les échantillons perdus restent visibles par un saut de t_us.
  if ((int32_t)(t - nextSampleUs) > 0) nextSampleUs = t + PERIOD_US;

  // Demande les 6 octets d'accélération à partir de ACCEL_XOUT_H
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(REG_ACCEL_XOUT_H);
  if (Wire.endTransmission(false) != 0 ||
      Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)6) != 6) {
    Serial.println("# Lecture MPU echouee");
    return;                              // échantillon manquant : visible par un saut de t_us
  }

  // Lecture dans l'ordre : X_H, X_L, Y_H, Y_L, Z_H, Z_L
  uint8_t b[6];
  for (int i = 0; i < 6; i++) b[i] = Wire.read();

  // Assemblage : octet haut décalé de 8 bits + octet bas (entier signé 16 bits)
  int16_t rawX = (int16_t)((b[0] << 8) | b[1]);
  int16_t rawY = (int16_t)((b[2] << 8) | b[3]);
  int16_t rawZ = (int16_t)((b[4] << 8) | b[5]);

  // Ligne CSV : t_us,ax_g,ay_g,az_g
  Serial.print(t);                      Serial.print(',');
  Serial.print(rawX / LSB_PER_G, 5);    Serial.print(',');
  Serial.print(rawY / LSB_PER_G, 5);    Serial.print(',');
  Serial.println(rawZ / LSB_PER_G, 5);
}
