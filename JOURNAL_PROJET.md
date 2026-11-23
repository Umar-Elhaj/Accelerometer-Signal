# Journal de projet — Analyse des mouvements avec un accéléromètre

> Projet 5 — Module Traitement du signal (Master 1, Sem 7)
> Durée indicative : 4 semaines à partir de la validation du sujet.
> Ce journal trace chaque ajout, retrait, décision et avancement, pour servir de base au rapport.
> **Les décisions sont prises par le chef de projet** ; Claude propose, documente et implémente.

---

## 1. Rappel du sujet

**Contexte :** un accéléromètre mesure l'accélération selon plusieurs axes et permet de caractériser des mouvements et vibrations.

**Matériel / logiciels :** module MPU6050 (ou équivalent), Arduino/ESP32, ordinateur.

**Consignes d'acquisition :**
- Connecter le capteur et acquérir X, Y, Z à **fréquence fixe**.
- Enregistrer 4 situations : **immobile**, **inclinaison lente**, **rotation contrôlée**, **petites oscillations manuelles**.
- **3 répétitions** par situation, **10 à 20 s** chacune, export **CSV**.

**Traitement :** tracer les 3 axes, calculer éventuellement la norme, comparer les situations, appliquer un filtre de lissage ou passe-bas.

**Livrables :** schéma, code, fichiers CSV, graphiques avant/après, tableau comparatif.

**Précautions :** fixer solidement le capteur, éviter chocs et mouvements brusques.

---

## 2. Checklist des exigences (consignes communes + sujet)

| # | Exigence | Statut |
|---|----------|--------|
| 1 | Présenter le problème, son intérêt, les objectifs | ⬜ À faire |
| 2 | Décrire le signal (grandeur, unité, durée, Fe, nb d'échantillons) | ⬜ À faire |
| 3 | Décrire matériel, logiciel, source des données, protocole | ⬜ À faire |
| 4 | Fournir les données brutes (CSV), **fichiers distincts des données filtrées** | ⬜ À faire |
| 5 | Visualiser le signal brut (temporel + fréquentiel si pertinent) | ⬜ À faire |
| 6 | Identifier perturbations, bruit, caractéristiques | ⬜ À faire |
| 7 | Appliquer ≥ 1 méthode de traitement du cours, justifier les paramètres | ⬜ À faire |
| 8 | Comparer avant/après (graphiques + indicateurs) | ⬜ À faire |
| 9 | Interpréter, limites, amélioration proposée | ⬜ À faire |
| S1 | Schéma de câblage | ⬜ À faire |
| S2 | Code d'acquisition commenté et réexécutable | 🟡 En cours (sketch + notebook prêts, à tester sur la carte) |
| S3 | 4 situations × 3 répétitions × 10–20 s = 12 CSV | ⬜ À faire |
| S4 | Graphiques : titre, axes nommés, unités, légende | ⬜ À faire |
| S5 | Tableau comparatif des situations | ⬜ À faire |

Légende : ⬜ À faire · 🟡 En cours · ✅ Fait

### 2.1 Structure obligatoire du rapport

| Section | Contenu attendu | Statut |
|---------|-----------------|--------|
| Page de garde | Titre, noms des membres, filière, date | ⬜ |
| Introduction | Contexte, problématique, objectifs | ⬜ |
| Matériel et méthode | Composants, logiciels, schéma, protocole détaillé | ⬜ |
| Description des données | Format, durée, Fe, unités, nb d'échantillons | ⬜ |
| Méthodes de traitement | Algorithmes et paramètres | ⬜ |
| Résultats | Graphiques lisibles, tableaux, avant/après | ⬜ |
| Discussion | Interprétation, difficultés, limites, améliorations | ⬜ |
| Conclusion + références | — | ⬜ |
| Annexes | Code source, schémas, fichiers complémentaires | ⬜ |

### 2.2 Éléments à remettre

| Élément | Statut |
|---------|--------|
| Rapport final (format demandé par l'enseignant) | ⬜ |
| Données brutes **et** données traitées, fichiers distincts | ⬜ |
| Code source complet + fichiers nécessaires à l'exécution | ⬜ |
| Schéma de câblage | ⬜ |
| Présentation synthétique pour la soutenance | ⬜ |

---

## 3. Décisions

| Date | Décision | Justification | Décidé par |
|------|----------|---------------|------------|
| 2026-11-11 | Création du journal de projet | Traçabilité pour le rapport | Chef de projet |
| 2026-11-11 | Matériel : ESP32-C3 + MPU-6050 + fils + breadboard | Matériel disponible, conforme au sujet | Chef de projet |
| 2026-11-11 | Câblage I²C : SDA → GPIO 5, SCL → GPIO 6 | Câblage réel (le commentaire « 8/9 » du sketch est faux) | Chef de projet |
| 2026-11-11 | Analyse en Python sous **Jupyter Notebook** | Préférence de travail du chef de projet | Chef de projet |
| 2026-11-11 | Fréquence d'échantillonnage **Fe = 100 Hz**, cadencée par `micros()` | Fréquence fixe exigée par le sujet ; Fe/2 = 50 Hz couvre largement les mouvements manuels (< 10–15 Hz) et évite le repliement | Chef de projet |
| 2026-11-11 | Plage de mesure **±2 g** (16384 LSB/g) | Mouvements lents (on reste sous 2 g) → résolution maximale (≈ 0,06 mg) | Chef de projet |
| 2026-11-11 | Modifier directement `test_mpu.ino` (pas de nouveau sketch) | Simplicité | Chef de projet |
| 2026-11-11 | Collecte des données via un notebook Python (`pyserial`) | Préférence notebooks ; enregistrement automatique en CSV | Chef de projet |
| 2026-11-11 | Filtre passe-bas interne du MPU (DLPF) à **44 Hz** | Anti-repliement sous Fe/2 = 50 Hz | Chef de projet (validé) |
| 2026-11-11 | 3 répétitions **identiques** par situation (même durée, même geste) ; durée **15 s** (1 500 échantillons) | Comparabilité et reproductibilité ; 15 s est au milieu de la plage 10–20 s | Chef de projet (validé) |
| 2026-11-21 | Essai invalide conservé comme trace dans `data/raw/rejets/` (renommé `immobile_rep1_rafale.csv`) au lieu d'être supprimé | Règle : ne jamais détruire une donnée brute ; exemple utile pour la partie « Difficultés » du rapport | Chef de projet |
| 2026-11-21 | Sketch remis à `FS_HZ = 100` | Valeur décidée le 11/11 (une valeur de 1 Hz avait été laissée dans le code) | Chef de projet |
| 2026-11-21 | Le notebook **lit le port en continu pendant le décompte** (plus de `time.sleep`) | Supprime la cause des rafales : l'ESP32 n'est plus jamais bloqué en envoi | Chef de projet |
| 2026-11-21 | Légère inclinaison au repos acceptée (capteur sur breadboard, surface non parfaitement plane) | Limite matérielle connue, quantifiée par les essais « immobile » et discutée dans le rapport | Chef de projet |
| 2026-11-21 | **Protocole « inclinaison » = rotation autour de l'axe Y** (celle réalisée en rep1) : départ X vers le haut → ≈ 90° (Z vers le bas) → retour ; rep2 et rep3 à l'identique | Rep1 valide et exploitable ; l'important est la reproductibilité entre répétitions | Chef de projet |
| 2026-11-21 | Chronomètre (0 à 15 s) + consignes affichés pendant l'enregistrement | Faciliter le changement d'action au bon moment et rendre les répétitions identiques | Chef de projet |
| 2026-11-21 | **Protocole « rotation » = demi-tour (180°) aller-retour autour de Y**, vitesse constante 90° / 2 s | Le câble USB sort selon Y → rotation « en broche » sans enrouler le câble ; demi-tour plus simple à maîtriser qu'un tour complet (essai) | Chef de projet |
| 2026-11-21 | **Protocole « oscillations » = glissement va-et-vient sur la table**, course **3 cm** (A = 1,5 cm) selon Y, **f = 1 Hz**, de 2 à 15 s | Seule situation où le capteur mesure une accélération de mouvement (la gravité reste fixe) ; fréquence et amplitude connues → vérification théorique a_max = A(2πf)² ≈ 0,06 g et pic attendu à 1 Hz dans la FFT | Chef de projet |
| 2026-11-21 | Rythme donné par le notebook (ALLER / RETOUR toutes les 0,5 s) au lieu d'un métronome externe | Pas d'application installable ; le rythme est cadencé par l'horloge de l'ESP32 | Chef de projet |

---

## 4. Journal chronologique

### 2026-11-11 — Démarrage
- **Ajout** : `JOURNAL_PROJET.md` (ce fichier).
- **État initial** : un sketch de test existe déjà, [test_mpu/test_mpu.ino](test_mpu/test_mpu.ino).
  - Carte : **ESP32-C3**, capteur **MPU-6050** en I²C (400 kHz), adresse 0x68.
  - Initialisation : vérification de présence, lecture `WHO_AM_I`, réveil du capteur, plage **±8 g** (4096 LSB/g).
  - Boucle : lecture des 6 octets d'accélération, conversion en g, calcul de la norme |a|, affichage sur le moniteur série (format tabulé).
- **Points relevés dans le code de test** (en attente de décision, rien n'a été modifié) :
  1. **Incohérence de broches** : le commentaire indique SDA→GPIO 8 / SCL→GPIO 9, mais le code utilise `SDA_PIN 5` / `SCL_PIN 6`. À vérifier avec le câblage réel.
  2. **Fréquence non fixe** : `delay(100)` donne ≈ 10 Hz mais avec dérive (le temps de lecture/affichage s'ajoute). Le sujet exige une fréquence fixe → cadencement par `micros()` ou timer à envisager, et ajout d'un horodatage par échantillon.
  3. **Plage ±8 g** choisie pour des chocs/chutes ; le sujet porte sur des mouvements lents (inclinaison, rotation, petites oscillations) → **±2 g** offrirait une résolution 4× meilleure (16384 LSB/g).
  4. **Format de sortie** : tabulations + texte libre ; pas directement un CSV exploitable (pas de colonne temps, pas de séparateur virgule).
  5. **Précision d'affichage** : 2 décimales (0,01 g) — plus grossier que la résolution du capteur ; perte d'information pour l'analyse du bruit.
  6. **Filtre passe-bas interne (DLPF, registre 0x1A)** non configuré : à décider selon Fe (anti-repliement).

### 2026-11-11 — État de l'art fourni par le chef de projet
- **Ajout au journal** : structure obligatoire du rapport (§2.1) et éléments à remettre (§2.2).
- **Matériel confirmé** : ESP32-C3, MPU-6050, fils de connexion, breadboard.
- **Câblage confirmé** : SDA → GPIO 5, SCL → GPIO 6 (conforme au code ; le commentaire d'en-tête « GPIO 8/9 » est à corriger).
- **Acquisition fonctionnelle** : les données s'affichent dans le moniteur série de l'Arduino IDE, une ligne toutes les ~0,1 s (≈ 10 Hz).
- **Analyse Fe = 10 Hz** : le flux paraît « rapide » à l'œil, mais 10 Hz est **faible** en traitement du signal. D'après Shannon, on ne peut observer que des fréquences < Fe/2 = 5 Hz ; les petites oscillations manuelles et surtout le bruit au-delà de 5 Hz seraient **repliés** (aliasing) dans la bande utile. Le moniteur série n'est qu'un affichage : les données seront enregistrées par un script et non lues à l'œil.
- **Situations à enregistrer (consigne 23)** : 4 situations × 3 répétitions × 10–20 s → **12 fichiers CSV bruts** :
  1. Immobile (référence : bruit du capteur, offset, gravité ≈ 1 g sur un axe)
  2. Inclinaison lente
  3. Rotation contrôlée
  4. Petites oscillations manuelles
- **Remarque physique (rotation contrôlée)** : un accéléromètre seul ne « voit » une rotation que si elle change l'orientation de la gravité par rapport au capteur. Une rotation autour de l'axe **vertical** (capteur à plat qui tourne comme un plateau) est quasi invisible : Z reste à ≈ 1 g. → La rotation doit se faire autour d'un **axe horizontal** (ou il faudrait aussi le gyroscope du MPU-6050, ce qui dépasse le sujet).
- **Prochaine étape** : collecte des données vers CSV via Python (en attente des décisions sur Fe, plage, protocole).

### 2026-11-11 — Sketch d'acquisition + notebook de collecte
- **Décisions** : Fe = 100 Hz, plage ±2 g, modification directe du sketch, notebook Python pour la collecte (voir §3).
- **Modification** : [test_mpu/test_mpu.ino](test_mpu/test_mpu.ino). *Le fichier a été mis à jour pendant la session (version constatée dans le dossier, conforme aux décisions, conservée telle quelle).* Contenu :
  - cadencement à **100 Hz fixe** par `micros()` ; l'instant suivant est planifié depuis l'instant prévu → pas de dérive cumulée (contrairement à `delay(100)`) ;
  - plage **±2 g** (registre 0x1C = 0x00, 16384 LSB/g) ;
  - **DLPF 44 Hz** (registre 0x1A = 0x03) comme filtre anti-repliement ;
  - sortie **CSV** `t_us,ax_g,ay_g,az_g` avec 5 décimales ; messages d'information préfixés par `#` ;
  - **retrait** du calcul de la norme et de la conversion en m/s² côté carte → faits en Python, pour que le CSV ne contienne que des données brutes.
- **Ajout** : [notebooks/01_acquisition.ipynb](notebooks/01_acquisition.ipynb) :
  - liste les ports série, puis enregistre exactement `DUREE_S × 100` échantillons après un décompte de 3 s ;
  - sauvegarde dans `data/raw/<situation>_rep<n>.csv` (colonnes `t_us, t_s, ax_g, ay_g, az_g`) ;
  - **refuse d'écraser** un fichier brut existant (règle du projet) ;
  - ajoute une ligne dans `data/raw/journal_acquisitions.csv` : date, durée, nb d'échantillons, Fe effective, écart min/max entre échantillons, échantillons manquants → sert pour « Description des données » dans le rapport ;
  - trace les 3 axes (titre, axes, unités, légende) pour le contrôle immédiat ;
  - affiche le tableau d'avancement des 12 enregistrements.
- **Test** : logique du notebook vérifiée avec un port série simulé (1 500 échantillons, gestion du débordement de `micros()`, détection d'un échantillon manquant, protection contre l'écrasement). **Pas encore testé avec la carte réelle.**
- **Prérequis** : `pip install pyserial` (cellule prévue dans le notebook) ; fermer le moniteur série de l'Arduino IDE avant d'enregistrer.
- **Validation** du chef de projet : DLPF 44 Hz et durée de 15 s par essai (3 répétitions identiques).

### 2026-11-11 — Premier essai sur la carte : erreur de port (corrigée)
- **Problème** : `SerialException: could not open port 'COM3'` alors que la carte est sur **COM5**.
- **Cause** : dans `acquerir(..., port=PORT)`, la valeur par défaut est figée par Python **au moment de la définition** de la fonction (alors `COM3`) ; modifier `PORT` ensuite n'avait aucun effet sans réexécuter la cellule de la fonction.
- **Correction** dans [notebooks/01_acquisition.ipynb](notebooks/01_acquisition.ipynb) : `PORT = "COM5"` ; `port` et `duree_s` valent maintenant `None` par défaut et sont lus au moment de l'appel.
- **À retenir pour le rapport (difficultés)** : un port série ne peut être ouvert que par un seul programme (moniteur série Arduino fermé obligatoirement).

### 2026-11-11 — Aucune donnée reçue (TimeoutError) : diagnostic et correction
- **Symptôme** : le port COM5 s'ouvre mais aucune ligne de données n'arrive (même après redémarrage du PC, Arduino IDE fermé).
- **Diagnostic** (tests directs sur COM5) :
  1. Avec l'ouverture par défaut de `pyserial` (DTR et RTS activés), **0 ligne** : sur l'ESP32-C3 à USB natif, DTR/RTS pilotent le reset de la puce → la carte est **maintenue en reset**.
  2. Avec DTR/RTS désactivés, la carte redémarre (messages de boot reçus : `mode:DIO … entry 0x403cbf10`) mais le programme **n'envoie ensuite rien sur l'USB**.
  3. Les options de compilation de l'Arduino IDE montrent la cause : le dernier téléversement (11/11, 16:27) a été fait avec **`USB CDC On Boot = Disabled`** (`CDCOnBoot=default`) et `Flash Mode = QIO`, alors que la version qui fonctionnait (09/11) utilisait **`USB CDC On Boot = Enabled`** et `Flash Mode = DIO`. Avec CDC désactivé, `Serial` sort sur les broches UART (GPIO 20/21) et non sur le port USB. Les réglages de carte ont été perdus parce que le sketch a été ouvert depuis un autre dossier.
- **Corrections** :
  - Arduino IDE : **Outils → USB CDC On Boot → Enabled**, **Flash Mode → DIO**, puis téléverser à nouveau (action du groupe).
  - Notebook : ouverture du port avec `dtr = rts = False` ; attente portée à 8 s ; en cas d'échec, le message affiche les dernières lignes reçues (diagnostic). Notice « Avant de lancer » complétée.
- **À retenir pour le rapport (difficultés)** : spécificités de l'USB natif de l'ESP32-C3 (signaux DTR/RTS, option USB CDC On Boot).

### 2026-11-11 — Premier enregistrement réel (`immobile_rep1.csv`) : essai invalide
- **Résultat** : la chaîne fonctionne (1 500 échantillons reçus), mais l'essai est **invalide** :
  - durée totale 3,72 s au lieu de 15 s ; Fe effective calculée 403 Hz ;
  - écart entre échantillons ≈ **0,6 ms** au lieu de 10 ms (≈ 1,6 kHz) ; 581 lignes identiques consécutives (le capteur ne se met à jour qu'à 1 kHz) ;
  - un **trou de 2,81 s** entre les échantillons 7 et 8 → segment « lisse » de 0 à 2,8 s sur le graphique (simple droite entre deux points).
- **Cause 1 (sketch)** : quand aucun programme n'a ouvert le port, l'envoi série bloque et la boucle prend du retard sur son planning ; à l'ouverture du port, le sketch **rattrapait ce retard en rafale** à vitesse maximale.
  → **Correction** [test_mpu.ino](test_mpu/test_mpu.ino) : si le retard dépasse une période, on se recale (`nextSampleUs = t + PERIOD_US`) au lieu de rattraper. Les échantillons perdus restent visibles par un saut de `t_us`.
- **Cause 2 (notebook)** : quelques anciens échantillons étaient encore « en transit » dans l'USB après `reset_input_buffer()`.
  → **Correction** [01_acquisition.ipynb](notebooks/01_acquisition.ipynb) : purge de 0,5 s après le décompte ; ajout d'un **contrôle automatique de validité** (alerte si Fe effective hors 100 Hz ± 2 % ou échantillons manquants).
- **Observation physique intéressante** (malgré l'essai invalide) : X ≈ 0,978 g, Y ≈ 0, Z ≈ −0,22 g → norme ≈ √(0,978² + 0,22²) ≈ **1,00 g** : le capteur mesure bien la gravité seule. Le capteur n'était pas à plat : l'axe X était presque vertical (inclinaison ≈ 13° par rapport à la verticale). → L'orientation de repos doit être fixée et documentée dans le protocole.
- **En attente de décision** : sort du fichier invalide `immobile_rep1.csv` (la règle interdit d'écraser un fichier brut).

### 2026-11-20 — Analyse de l'enregistrement du 12/11 (`immobile_rep1.csv`) : toujours une rafale au départ
- **Constat du chef de projet** : « l'enregistrement part trop vite après le décompte ».
- **Mesures** (`journal_acquisitions.csv` + analyse des `t_us`) : 1 500 échantillons en **11,49 s** au lieu de 15 s, Fe effective **130,5 Hz**.
  - Les **374 premiers échantillons** arrivent en **0,23 s** (un toutes les ≈ 0,615 ms ≈ 1,6 kHz) ;
  - ensuite, l'échantillonnage est parfaitement régulier : écart **10,000 ms** (min 9,997 / max 10,003 ms).
  - 374 × 10 ms ≈ 3,7 s ≈ décompte (3 s) + purge (0,5 s) : c'est exactement le retard accumulé pendant que le notebook ne lisait pas le port.
- **Cause** : même mécanisme que l'essai du 11/11 (rattrapage du retard en rafale par le sketch). Le recalage est présent dans le code source, mais la carte exécutait très probablement une version téléversée **avant** cette correction (non vérifiable a posteriori).
- **Conséquence (traitement du signal)** : la période d'échantillonnage n'est pas constante au début du fichier → les outils (FFT, filtres) qui supposent Te constant ne sont pas applicables tels quels → **essai invalide**.
- **Constat annexe** : le sketch contient actuellement `FS_HZ = 1` (version compilée le 18/11) ; à remettre à 100 avant les enregistrements.
- **Décisions** : voir 2026-11-21 ci-dessous.

### 2026-11-21 — Corrections après l'essai invalide du 12/11
- **Déplacement** : `data/raw/immobile_rep1.csv` → [data/raw/rejets/immobile_rep1_rafale.csv](data/raw/rejets/immobile_rep1_rafale.csv) (contenu non modifié). La ligne correspondante reste dans `journal_acquisitions.csv` (historique complet ; elle porte le nom d'origine `immobile_rep1.csv`).
- **Modification** [test_mpu.ino:54](test_mpu/test_mpu.ino#L54) : `FS_HZ = 1` → `FS_HZ = 100`. **À téléverser sur la carte** (action du groupe).
- **Modification** [01_acquisition.ipynb](notebooks/01_acquisition.ipynb), fonction `acquerir` : pendant le décompte, le notebook **continue à lire** le port et jette les lignes reçues (au lieu de `time.sleep(1)`). La purge de 0,5 s devient inutile et a été retirée.
  - Principe : un flux série doit être **lu en permanence**. S'il ne l'est pas, l'émetteur (l'ESP32) est bloqué et l'échantillonnage n'est plus régulier.
- **Test** (port série simulé, une ligne toutes les 10 ms en temps réel) : 500 échantillons en 4,99 s, Fe effective **100,0 Hz**, écart constant 10,0 ms, **0 échantillon manquant**. **Reste à valider sur la carte réelle.**

### 2026-11-21 — Observation au moniteur série : orientation du capteur au repos
- Lignes relevées : `60107857,-0.24976,-0.01270,-0.92725` et `61107858,-0.25073,-0.01221,-0.93091`.
- **Fe** : écart entre les deux `t_us` = 1 000 001 µs → **1 Hz** : la carte exécute encore l'ancienne version (`FS_HZ = 1`) → le sketch corrigé n'est pas encore téléversé.
- **Orientation** : a_z ≈ −0,93 g → l'axe Z pointe **vers le bas** (capteur retourné, composants vers la table) ; a_x ≈ −0,25 g → capteur **incliné d'environ 15°** (≈ atan(0,25 / 0,93)) ; a_y ≈ 0.
- **Norme** : √(0,250² + 0,012² + 0,930²) ≈ **0,96 g** au lieu de 1 g → écart ≈ 4 % : décalages (offsets) propres au capteur, à quantifier avec les essais « immobile ».
- **À décider** : position de départ de référence (à plat, Z vers le haut ou vers le bas) et vérification de l'horizontalité.

### 2026-11-21 — Axes identifiés, début des essais « immobile »
- **Axes X, Y, Z identifiés** par le chef de projet sur le montage.
- **Décision** : le capteur est monté sur la **plaque d'essai (breadboard)**, dont la surface n'est pas parfaitement plane → une légère inclinaison au repos est **acceptée** et sera **mentionnée dans le rapport** (partie Discussion / limites). Elle sera quantifiée par les essais « immobile ».
- **Démarrage** des enregistrements par la situation **immobile** (3 répétitions, 15 s).

### 2026-11-21 — Situation 1 « immobile » : 3 répétitions enregistrées ✅
- Fichiers : `data/raw/immobile_rep1.csv`, `immobile_rep2.csv`, `immobile_rep3.csv` (17:50 – 17:53).
- **Orientation** : capteur à plat, **Z vers le haut** (a_z ≈ +1,03 g).
- **Qualité de l'échantillonnage** (les 3 essais) : 1 500 échantillons, 14,99 s, Fe effective **100,0 Hz**, **0 échantillon manquant**. 6 écarts irréguliers sur 1 499 (un échantillon décalé de 4 ms : 14 ms puis 6 ms), écart-type de Te ≈ 0,2 ms → négligeable, à citer dans le rapport.
- **Premiers indicateurs** :

| Essai | moy. a_x (g) | moy. a_y (g) | moy. a_z (g) | σ_x / σ_y / σ_z (mg) | moy. \|a\| (g) | dérive a_x sur 15 s (mg) |
|---|---|---|---|---|---|---|
| rep1 | 0,177 | −0,012 | 1,027 | 5,3 / 4,6 / 4,2 | 1,042 | −1,6 |
| rep2 | 0,157 | −0,008 | 1,029 | 4,4 / 2,2 / 3,7 | 1,041 | −6,7 |
| rep3 | 0,174 | −0,009 | 1,026 | 5,3 / 2,2 / 3,7 | 1,041 | −12,2 |

- **Interprétation provisoire** :
  - les lignes sont plates : les petites fluctuations (σ ≈ 2 à 5 mg) sont le **bruit du capteur**, pas un mouvement ;
  - a_x ≈ 0,17 g constant = **inclinaison** de ≈ 10° (atan(0,17/1,03)) due à la breadboard, pas un mouvement ;
  - |a| ≈ 1,04 g (Z vers le haut) contre ≈ 0,96 g le même jour avec Z vers le bas → cohérent avec un **décalage (offset) d'environ +0,04 g sur l'axe Z** ;
  - **dérive lente** de a_x (jusqu'à −12 mg en 15 s) et moyenne de a_x différente entre rep2 et rep1/rep3 (≈ 20 mg) : hypothèses = dérive thermique du capteur après la mise sous tension, ou léger déplacement/tension du câble entre les essais. À discuter dans le rapport.

### 2026-11-21 — Situation 2 « inclinaison » : répétition 1 enregistrée (à valider)
- **Protocole proposé** : départ immobile (0–2 s), inclinaison lente jusqu'à 90° (≈ 5 s), maintien (≈ 3 s), retour lent (≈ 4 s), immobile ; rotation prévue **autour de X**, butée contre un objet vertical.
- Fichier `data/raw/inclinaison_rep1.csv` (18:09) : 1 500 échantillons, 14,99 s, Fe **100,0 Hz**, 0 manquant → **techniquement valide**.
- **Déroulé observé** : immobile 0–3 s, montée 3–8 s, maintien 8–11 s, retour 11–14,9 s → conforme au déroulé prévu.
- **Écart au protocole** : ce sont **a_x et a_z** qui varient, **a_y reste ≈ 0** (±0,03 g) → la rotation s'est faite **autour de l'axe Y**, pas de X. Position de départ : a_x ≈ +0,99 g, a_z ≈ −0,15 g (**X vers le haut**) → différente de celle des essais « immobile » (Z vers le haut). Position finale : a_z ≈ −0,96 g, a_x ≈ −0,08 g (**Z vers le bas**). Angle parcouru ≈ **86°** (de ≈ 9° à ≈ 95°, calculé par atan2(−a_z, a_x)).
- **Confirmation du décalage sur Z** : |a| ≈ 1,00 g quand X porte la gravité, ≈ 0,96 g quand Z est vers le bas (et ≈ 1,04 g Z vers le haut, essais « immobile ») → offset ≈ **+0,04 g sur Z**, cohérent sur deux situations.
- **Perturbations** : deux petits pics vers 9,1 s et 11,3 s (contact avec la butée / la main pendant le maintien) ; petites ondulations pendant la montée et la descente (accélérations de la main qui s'ajoutent à la gravité).
- **Décision** : on **garde** ce mouvement comme protocole (rotation **autour de Y**, de « X vers le haut » à « Z vers le bas ») ; rep1 est conservée, rep2 et rep3 seront faites à l'identique.

### 2026-11-21 — Chronomètre et consignes pendant l'enregistrement
- **Demande du chef de projet** : voir les secondes défiler (1 à 15 s) pour changer d'action au bon moment.
- **Modification** [01_acquisition.ipynb](notebooks/01_acquisition.ipynb) :
  - cellule « Paramètres » : ajout du dictionnaire `GUIDE` {seconde : action} ;
  - fonction `acquerir` : à chaque fois que 100 mesures sont reçues (= 1 s selon l'horloge de l'ESP32), affichage du temps écoulé et de l'action à commencer.
- **Protocole « inclinaison »** (instants repris de rep1) : 0 s immobile (X vers le haut) → **3 s** monter lentement → **8 s** maintenir ≈ 90° contre la butée → **11 s** redescendre lentement → **14 s** immobile → 15 s fin.
- **Test** (port simulé) : affichage de 0 à 15 s avec les consignes aux bons instants ; 1 500 échantillons, Fe 100,0 Hz, 0 manquant.

### 2026-11-21 — Protocole de la situation 3 « rotation contrôlée »
- **Contrainte** : capteur sur breadboard alimenté par câble USB → pas de rotation continue (le câble s'enroulerait) ; un accéléromètre ne voit une rotation que si l'axe de rotation est **horizontal**.
- **Solution** : rotation « en broche de rôtisserie » autour de l'axe **dans l'alignement du câble USB** = **axe Y** du capteur (identifié par le chef de projet) → le câble se vrille d'un demi-tour au lieu de s'enrouler.
- **Choix** : **demi-tour (180°) aller-retour**, vitesse constante **90° toutes les 2 s (45°/s)** :
  0 s immobile (même départ que l'inclinaison) → 2 s tourner (aller) → 4 s à 90° → 6 s à 180°, maintien → 8 s tourner (retour) → 10 s à 90° → 12 s position de départ, immobile → 15 s fin.
- **Différences avec l'inclinaison** (même axe Y) : amplitude **180°** au lieu de 90°, vitesse **imposée et constante** (repères tous les 90°), aller-retour symétrique.
- **Signal attendu** : a_y ≈ constant (≈ 0, axe de rotation) ; a_x de ≈ +1 g → 0 → ≈ −1 g puis retour (changement de signe) ; a_z de ≈ 0 → ≈ −1 g (à 90°) → ≈ 0 ; |a| ≈ 1 g.
- **Modification** [01_acquisition.ipynb](notebooks/01_acquisition.ipynb) : ajout des consignes `"rotation"` dans `GUIDE` (testé avec le port simulé).

### 2026-11-21 — Situation 2 « inclinaison » : 3 répétitions valides ✅
- **Remarque de traçabilité** : d'après `journal_acquisitions.csv`, plusieurs essais ont été refaits (rep1 : 18:09, 18:23, 18:23, 18:24 ; rep2 : 18:15, 18:22, 18:24, 18:25). Les versions précédentes ont été **supprimées** (non déplacées dans `rejets/`). Les fichiers retenus sont rep1 (18:24:26), rep2 (18:25:29) et rep3 (18:26:04). Le fichier rep1 de 18:09 analysé plus haut n'existe plus.
- **Qualité** : 3 × 1 500 échantillons, Fe **100,0 Hz**, 0 manquant.
- **Résultats** (angle calculé dans le plan XZ par atan2(−a_z, a_x), relatif au départ) :

| Essai | départ (a_x ; a_z) | angle max | maintien | retour à 0° | a_y (min/max) | \|a\| (min/max) |
|---|---|---|---|---|---|---|
| rep1 | 0,96 ; −0,28 g | 94,4° | 8 → 11 s | ≈ 15 s | −0,06 / +0,03 g | 0,90 / 1,02 g |
| rep2 | 0,96 ; −0,28 g | 95,9° | 7 → 11 s | ≈ 15 s | −0,04 / +0,01 g | 0,95 / **1,24 g** |
| rep3 | 0,96 ; −0,28 g | 95,8° | 9 → 11 s | ≈ 15 s | −0,04 / +0,05 g | 0,88 / 1,15 g |

- **Interprétation** : très bonne **reproductibilité** (même départ, même angle final ≈ 95°, même déroulé) ; a_y ≈ 0 confirme la rotation autour de Y. Pics ponctuels de \|a\| (rep2 à 3 s : 1,24 g ; rep3 vers 1,5 s et 3 s) = **à-coups** de la main au démarrage du mouvement → perturbations impulsionnelles à traiter par le filtrage.

### 2026-11-21 — Situation 3 « rotation » : rep1 non conforme
- Fichier `rotation_rep1.csv` (18:39:37, une version précédente de 18:37:33 a été supprimée) : 1 500 échantillons, Fe 100,0 Hz, 0 manquant.
- **Constat** : l'orientation du capteur par rapport à la gravité **ne change pas** pendant tout l'essai : a_x ≈ +0,96 g, a_z ≈ −0,26 g, a_y ≈ 0 de 0 à 15 s ; angle dans le plan XZ compris entre −6° et +12° (au lieu de 0 → 180° → 0). Seules apparaissent des vibrations de la main (a_y jusqu'à ±0,13 g, \|a\| de 0,89 à 1,11 g) à partir de ≈ 1 s.
- **Interprétation** : la rotation s'est faite autour d'un axe **vertical** (l'axe X du capteur, qui porte la gravité), ou la plaque n'a pas tourné → **invisible pour un accéléromètre**, comme prévu par la théorie (cf. remarque du 11/11). Illustration expérimentale intéressante pour le rapport.
- **Proposition** : déplacer dans `rejets/` (sous le nom `rotation_rep1_axe_vertical.csv`) et refaire en gardant l'axe de rotation (le câble, axe Y) **horizontal** : même geste que l'inclinaison, mais poursuivi jusqu'à 180° (plaque à l'envers). **En attente de décision.**

### 2026-11-21 — Situation 3 « rotation » : nouvel essai rep1, rotation visible ✅
- **Geste du chef de projet** : plaque tenue **par le câble USB** entre les doigts (le câble sert d'axe horizontal), rotation aller-retour de 180°.
- Fichier `rotation_rep1.csv` (18:50:56) ; les versions de 18:39, 18:43 et 18:44 ont été **supprimées** (l'essai « axe vertical » décrit ci-dessus n'est donc plus disponible, seule sa description reste ici et dans `journal_acquisitions.csv`).
- **Qualité** : 1 500 échantillons, Fe 100,0 Hz, 0 manquant.
- **Résultat** : la rotation est **bien vue** par le capteur :
  - a_x passe de **+1,03 g à ≈ −0,93 g** puis revient à ≈ +1 g (changement de signe) ;
  - a_z : −0,26 g → **≈ −0,92 g** (90°, t ≈ 5 s) → **≈ +0,25 g** (180°, t ≈ 9 s) → retour ; à 180°, les composantes sont **inversées** par rapport au départ (+1,03 ; −0,26) → (−0,93 ; +0,25) : cohérent avec un demi-tour ;
  - a_y ≈ 0 la plupart du temps (axe de rotation), sauf pics jusqu'à +0,35 g vers 12–13 s (doigts sur le câble) ;
  - angle dans le plan XZ : 0° → **≈ 195°** (léger dépassement) → ≈ −10°.
- **Déroulé réel vs consignes** : 90° atteint à ≈ 5 s (prévu 4 s), 180° à ≈ 8 s (prévu 6 s), maintien très court (8–9 s au lieu de 6–8 s), retour à 0° vers 13,5 s (prévu 12 s) → environ **1 à 2 s de retard** sur le guide, vitesse moins régulière.
- **Perturbations** : oscillations de la plaque suspendue au câble (effet **pendule**), visibles sur \|a\| (0,87 à 1,16 g) au début et à la fin.
- **En attente de décision** : garder le guide actuel (A) ou l'adapter au rythme réel de rep1 (B).

### 2026-11-21 — Protocole de la situation 4 « petites oscillations »
- **Geste** : plaque posée sur une feuille de papier ; deux traits à **3 cm** l'un de l'autre ; le **bord avant** de la plaque va du trait 1 au trait 2 et revient, dans la direction du câble (**axe Y**), sans soulever ni tourner la plaque.
- **Déroulé** : 0–2 s immobile (référence), 2–15 s va-et-vient à **1 aller-retour par seconde (f = 1 Hz)**.
- **Pourquoi la distance est mesurée à la règle et non calculée** : retrouver la position par double intégration de l'accélération est irréalisable ici : l'offset mesuré (≈ 0,04 g ≈ 0,39 m/s²) donnerait après 15 s une erreur de ½ × 0,39 × 15² ≈ **44 m**. → limite à citer dans le rapport.
- **Signal attendu** : sinusoïde à 1 Hz sur a_y, amplitude théorique a_max = A(2πf)² = 0,015 × (2π)² ≈ 0,59 m/s² ≈ **0,06 g** ; a_x et a_z ≈ constants ; harmoniques possibles (geste manuel non sinusoïdal).
- **Modification** [01_acquisition.ipynb](notebooks/01_acquisition.ipynb) :
  - cellule « Paramètres » : consignes `"oscillations"` dans `GUIDE` + nouveau dictionnaire `METRONOME = {"oscillations": (2, 15)}` ;
  - fonction `acquerir` : pendant la fenêtre du métronome, affichage **ALLER / RETOUR toutes les 0,5 s** (comptées sur l'horloge de l'ESP32 : 50 mesures) ; le chronomètre affiche maintenant le temps au dixième (ex. `2.5 s`).
- **Test** (port simulé) : ALLER/RETOUR alternés de 2,0 à 14,5 s, 1 500 échantillons, Fe 100,0 Hz, 0 manquant.

### 2026-11-21 — Situation 4 « oscillations » : rep1 (premier essai)
- Fichier `oscillations_rep1.csv` (19:21:14 ; une version de 19:20:11 a été supprimée).
- **Qualité** : 1 500 échantillons, Fe effective 99,93 Hz, **1 échantillon manquant** (trou de 20 ms à t = 12,56 s) et 1 ligne corrompue ignorée → signalé « suspect » par le notebook ; impact très faible (1/1 500).
- **Écart-type** (repos 0–1 s → oscillations 2–15 s) : a_y **8,5 → 36,2 mg** ; a_x 14,2 → 12,0 mg ; a_z 5,6 → 16,7 mg → le mouvement est bien concentré sur **Y** (direction du glissement).
- **Signal brut** : beaucoup de composantes rapides (« grésillement » large bande) → à-coups de **frottement** de la plaque sur le papier (adhérence-glissement).
- **Après un passe-bas à 3 Hz** (Butterworth ordre 4, test exploratoire, pas encore la méthode retenue) : on voit nettement le va-et-vient ; amplitude ≈ **±0,04 g** (théorie ≈ 0,06 g pour 3 cm à 1 Hz → même ordre de grandeur).
- **Spectre (FFT de a_y, 2–15 s)** : pic à **1,23 Hz** (rythme réel du va-et-vient, plus rapide que 1 Hz) et pic à **≈ 0,4 Hz** (rythme irrégulier : le mouvement se répète par paquets d'environ 2,5 s) ; le mouvement commence vers 1,2 s au lieu de 2 s.
- **Hypothèse** : l'affichage ALLER/RETOUR dans VS Code peut arriver par à-coups (sortie du notebook mise en tampon) → rythme difficile à suivre.
- **En attente de décision** : garder rep1 ou la refaire ; moyen de mieux tenir le rythme (métronome Google dans le navigateur, compter à voix haute…).

---

## 5. Inventaire des fichiers

| Fichier | Rôle | Statut |
|---------|------|--------|
| `JOURNAL_PROJET.md` | Journal de bord du projet | Actif |
| `test_mpu/test_mpu.ino` | Sketch d'acquisition MPU-6050 sur ESP32-C3 (100 Hz, ±2 g, CSV) | Modifié le 2026-11-21 (`FS_HZ = 100`) |
| `notebooks/01_acquisition.ipynb` | Collecte série → CSV bruts + contrôle qualité | Modifié le 2026-11-21 (lecture continue pendant le décompte) |
| `data/raw/` | Données brutes (jamais modifiées) | 6/12 essais valides (immobile ×3, inclinaison ×3) ; rotation_rep1 non conforme |
| `data/raw/journal_acquisitions.csv` | Registre de toutes les acquisitions (y compris rejetées) | Actif |
| `data/raw/rejets/immobile_rep1_rafale.csv` | Essai invalide du 12/11 (rafale au départ), gardé comme trace | Rejeté |

---

## 6. Questions ouvertes / à valider

- [x] Broches I²C réellement utilisées → **5 (SDA) / 6 (SCL)**
- [x] Fréquence d'échantillonnage → **100 Hz**
- [x] Plage de mesure → **±2 g**
- [x] Méthode d'export CSV → **notebook Python + pyserial**
- [x] DLPF interne à 44 Hz → **validé**
- [x] Durée des enregistrements → **15 s pour tous** (validé)
- [x] Outil d'analyse → **Python, Jupyter Notebook**
- [ ] Protocole précis de chaque situation (position de départ, axe, amplitude, durée exacte) ?
- [ ] Filtre(s) à appliquer (moyenne glissante, Butterworth passe-bas, …) et paramètres ?
