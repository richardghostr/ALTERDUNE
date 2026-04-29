# ALTERDUNE — Mini-RPG Console en C++

> Projet de programmation orientée objet — ESILV, semestre 2 (2025-2026)
> Cours de Daniel Wladdimiro — Réalisé en binôme

---

## Qu'est-ce que ce projet ?

ALTERDUNE est un **jeu de rôle (RPG) jouable dans le terminal** (la fenêtre noire de votre ordinateur).
Vous incarnez un aventurier qui affronte des monstres, un par un, dans des combats au tour par tour.

L'originalité du jeu : **vous n'êtes pas obligé de tuer vos ennemis.**
Vous pouvez leur parler, les complimenter, leur faire des blagues... et les épargner.
À la fin, selon vos choix, vous obtenez une fin différente.

---

## Comment lancer le jeu ?

### Prérequis
- Un compilateur C++ compatible C++17 (exemple : `g++`)
- Un terminal (Linux, macOS, ou Windows avec WSL/MinGW)

### Compilation et lancement

Vous pouvez compiler et lancer le jeu de plusieurs manières selon votre environnement.

Option A — Utiliser le `Makefile` (recommandé)

```bash
# Depuis la racine du projet
# Linux / macOS
make

# Windows (MinGW / MSYS2)
mingw32-make

# Le binaire produit par la cible Makefile est :
# - ./alterdune (Unix)
# - alterdune.exe (Windows)
```

Option B — Compiler directement avec `g++` (PowerShell / CMD / bash)

PowerShell (Windows) — crée le dossier de sortie puis compile :

```powershell
if (!(Test-Path -Path src\output)) { New-Item -ItemType Directory -Path src\output }
g++ -std=c++17 -Wall -Wextra -Iinclude -g src/*.cpp -o src/output/main.exe
.\src\output\main.exe
```

Bash (Linux / macOS) :

```bash
mkdir -p src/output
g++ -std=c++17 -Wall -Wextra -Iinclude -g src/*.cpp -o src/output/main
./src/output/main
```

Remarques :
- Le `Makefile` (cible par défaut) produit un exécutable `alterdune` à la racine du projet sous Windows il sera nommé `alterdune.exe`.
- Lancez l'exécutable depuis la racine du projet afin que les fichiers `data/items.csv` et `data/monsters.csv` soient trouvés par `main`.
- Le binaire console force désormais l'encodage UTF-8 sous Windows au démarrage, vous n'avez normalement plus besoin d'exécuter `chcp 65001` manuellement.

**Déploiement GUI autonome (Windows)**

- Objectif : permettre d'exécuter la partie Qt/QML du jeu sur une machine cible même si Qt/MSYS n'y sont pas installés.

- Méthode recommandée (simple) — utiliser `windeployqt` fourni avec Qt. Exemple :

```powershell
.
scripts\deploy_windows.bat "C:\path\to\alterdune_qt.exe" "C:\path\to\ALTERDUNE\gui\qml"
```

Le script appelle `windeployqt --qmldir ...` pour rassembler automatiquement les DLL Qt, plugins (`platforms\qwindows.dll`) et les imports QML nécessaires.

- Fallback manuel (si `windeployqt` non disponible) — éléments à empaqueter près de l'exécutable :
	- Les DLL runtime MinGW (ex. `libstdc++-6.dll`, `libgcc_s_seh-1.dll` / `libgcc_s_dw2-1.dll`, `libwinpthread-1.dll`).
	- Les DLL Qt utilisées (ex. `Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Qml.dll`, `Qt6Quick.dll`, ... selon la build).
	- Le dossier `platforms` contenant `qwindows.dll`.
	- Le dossier `qml`/imports (ou le contenu généré par `windeployqt --qmldir`) pour que QML trouve ses modules.

- Remarques :
	- `windeployqt` doit correspondre à la même version de Qt que celle utilisée pour compiler l'exécutable.
	- La méthode la plus sûre est d'utiliser une build release et `windeployqt` pour éviter d'oublier un plugin.
	- Pour créer un installeur, utilisez NSIS, Inno Setup, ou l'outil Qt Installer Framework.


Option C — Interface graphique avec Qt (QML)

```bash
# Prérequis : Qt6 (Qt Quick) et CMake installés. Sur Windows, installez Qt via l'installateur officiel
# Depuis la racine du projet :
cd gui
mkdir -p build
cd build
# Générer les fichiers de build (exemple MinGW)
cmake -G "MinGW Makefiles" ..
# Compiler
cmake --build . --config Release
# L'exécutable produit : build/alterdune_qt (ou alterdune_qt.exe sous Windows)
./alterdune_qt
```

Remarque : l'application Qt Quick fournie dans `gui/` est un squelette QML minimal prêt à être intégré au moteur `Game`. Si vous souhaitez que j'intègre la logique C++ existante (`Game`, `FileLoader`, etc.) et l'expose à QML (boutons, vues de combat, bestiaire, inventaire), je peux le faire ensuite.

Pour repartir de zéro (supprimer les fichiers compilés) :
```bash
# Avec Makefile
make clean
# ou sous Windows avec MinGW
mingw32-make clean

# suppression manuelle (si vous avez compilé avec g++)
Remove-Item src\output\main.exe
```

---

## Structure du projet

```
ALTERDUNE-main/
│
├── include/          ← Déclarations des classes (.h)
├── src/              ← Code source des classes (.cpp)
├── data/             ← Fichiers de données du jeu
│   ├── items.csv         (objets de l'inventaire)
│   └── monsters.csv      (liste des monstres)
├── docs/             ← Diagrammes UML
├── Makefile          ← Script de compilation
└── readme.md         ← Ce fichier
```

---

## Comment jouer ?

### Démarrage
Au lancement, le jeu vous demande votre **nom de personnage**.
Il affiche ensuite un résumé : vos points de vie (HP) et votre inventaire de départ.

### Menu principal

```
=== MENU PRINCIPAL ===
1) Bestiaire
2) Démarrer un combat
3) Statistiques
4) Items
5) Quitter
```

| Option | Description |
|--------|-------------|
| Bestiaire | Consulte la liste de tous les monstres que vous avez vaincus |
| Démarrer un combat | Lance un combat contre un monstre tiré au hasard |
| Statistiques | Affiche votre nom, vos HP, vos kills, vos épargnes et vos victoires |
| Items | Consulte et utilise vos objets de soin hors combat |
| Quitter | Termine la partie |

### Objectif
Remportez **10 victoires** (tuer ou épargner 10 monstres).
À 10 victoires, le jeu se termine et affiche votre fin.

---

## Le système de combat

Chaque combat se déroule **tour par tour** : vous agissez, puis le monstre attaque.

```
-- Tour du joueur --
1) FIGHT   2) ACT   3) ITEM   4) MERCY
```

### FIGHT — Attaque directe
Vous attaquez le monstre. Les dégâts sont **aléatoires** (entre 0 et les HP max du monstre).
- Si vous tirez 0 : l'attaque rate.
- Si le monstre tombe à 0 HP : il est **tué**. Victoire +1.

### ACT — Interagir avec le monstre
Vous choisissez une action parmi celles disponibles pour ce monstre (2, 3 ou 4 selon son rang).
Chaque action affiche un texte drôle et **modifie la jauge Mercy** du monstre.

| Action | Effet sur Mercy |
|--------|----------------|
| COMPLIMENT | +15 (le monstre se calme) |
| JOKE | +10 |
| REASON | +12 |
| DANCE | +8 |
| OBSERVE | +5 |
| DISCUSS | +7 |
| OFFER_SNACK | +18 |
| PET | +14 |
| SHOUT | -10 (le monstre s'énerve) |
| INSULT | -20 |

### ITEM — Utiliser un objet
Vous utilisez un objet de votre inventaire pour vous soigner. Cela consomme votre tour.

### MERCY — Épargner le monstre
Si la jauge **Mercy >= 100**, vous pouvez épargner le monstre.
Il fuit sans mourir. Victoire +1, mais le monstre est **épargné** (pas tué).

---

## La jauge Mercy

Chaque monstre possède une jauge Mercy (entre 0 et 100).
Au départ elle est à **0**.
Utilisez des actions ACT positives pour la remplir.
Attention : certaines actions la **baissent** (INSULT, SHOUT).

```
Mercy : 45/100  ->  pas encore eparnable
Mercy : 100/100 ->  vous pouvez utiliser MERCY !
```

La jauge est toujours maintenue entre 0 et 100. Elle ne peut pas dépasser ni descendre sous ces bornes.

---

## Les monstres

Les monstres sont chargés depuis `data/monsters.csv`.
Il en existe 3 catégories, qui diffèrent par leur puissance et leur nombre d'actions ACT :

| Catégorie | Actions ACT | Exemples |
|-----------|------------|---------|
| NORMAL | 2 | Froggit, Gobbit, Slime |
| MINIBOSS | 3 | MimicBox, MimicKnight |
| BOSS | 4 | QueenByte, TitanCore |

### Tableau des monstres

| Nom | Catégorie | HP | ATK | DEF |
|-----|-----------|----|-----|-----|
| Froggit | NORMAL | 30 | 7 | 1 |
| Gobbit | NORMAL | 25 | 6 | 0 |
| Slime | NORMAL | 20 | 5 | 0 |
| MimicBox | MINIBOSS | 45 | 10 | 2 |
| MimicKnight | MINIBOSS | 60 | 12 | 3 |
| QueenByte | BOSS | 80 | 15 | 4 |
| TitanCore | BOSS | 120 | 20 | 6 |

---

## Les objets (Items)

Les objets sont chargés depuis `data/items.csv`.
Tous les objets sont de type **HEAL** : ils restaurent des points de vie.

| Objet | Soin | Quantité de départ |
|-------|------|--------------------|
| Bandage | +10 HP | 2 |
| Snack | +8 HP | 5 |
| Potion | +15 HP | 3 |
| SuperPotion | +30 HP | 1 |
| Elixir | +50 HP | 1 |

Les objets peuvent être utilisés **pendant le combat** (option ITEM) ou **depuis le menu Items** hors combat.

---

## Les fins multiples

Après 10 victoires, le jeu se termine avec l'une de ces trois fins :

| Fin | Condition | Description |
|-----|-----------|-------------|
| Fin Génocidaire | 0 monstre épargné | Vous avez tout tué. Le monde tremble. |
| Fin Pacifiste | 0 monstre tué | Vous avez tout épargné. La paix règne. |
| Fin Neutre | Mix tués + épargnés | Un équilibre fragile entre clémence et violence. |

---

## Formule de calcul des dégâts

Les dégâts sont tirés **aléatoirement** à chaque attaque :

```
dégâts = nombre aléatoire entre 0 et HP_max_de_la_cible
```

- Si dégâts = 0 : l'attaque rate
- Sinon : les HP de la cible diminuent de ce montant
- Les HP ne peuvent jamais descendre sous 0

Cette formule s'applique aussi bien pour le joueur que pour le monstre.

---

## Ajouter des monstres ou des objets

Vous pouvez modifier les fichiers CSV sans toucher au code !

### Ajouter un monstre dans `data/monsters.csv`

```
CATEGORIE;NOM;HP;ATK;DEF;MERCY_OBJECTIF;ACTION1;ACTION2;ACTION3;ACTION4
```

- CATEGORIE : NORMAL, MINIBOSS ou BOSS
- Les actions doivent exister dans le catalogue du jeu
- Mettez - pour les cases vides (un NORMAL n'utilise que 2 actions)

Exemple :
```
NORMAL;Crabbix;35;8;1;100;JOKE;OBSERVE;-;-
```

### Ajouter un item dans `data/items.csv`

```
NOM;HEAL;VALEUR;QUANTITE
```

Exemple :
```
MegaPotion;HEAL;60;1
```

---

## Architecture du code (pour les curieux)

Le projet utilise la **Programmation Orientée Objet (POO)** en C++.
Voici les grandes briques :

| Classe | Rôle |
|--------|------|
| Entity | Classe abstraite de base (nom, HP). Parente de tout. |
| Player | Le joueur. Hérite de Entity. Possède un inventaire. |
| Monster | Classe abstraite des monstres. Hérite de Entity. |
| Normal | Monstre de base, 2 actions ACT. |
| MiniBoss | Monstre intermédiaire, 3 actions ACT. |
| Boss | Monstre puissant, 4 actions ACT. |
| Item | Un objet de l'inventaire (nom, type, valeur, quantité). |
| Inventory | Liste d'items appartenant au joueur. |
| ActAction | Une action ACT (identifiant, texte, impact Mercy). |
| Bestiary | Journal des monstres vaincus. |
| FileLoader | Chargement des fichiers CSV au démarrage. |
| Game | Orchestrateur central : menus, combats, fins de partie. |

### Concepts POO utilisés

- **Encapsulation** : les données sont privées, accessibles uniquement via des méthodes
- **Héritage** : Player et Monster héritent d'Entity ; Normal, MiniBoss, Boss héritent de Monster
- **Polymorphisme** : actCount() retourne 2, 3 ou 4 selon le type de monstre
- **Composition** : le Player possède un Inventory, qui possède des Item

---

## Évaluation

| Étape | Poids | Contenu |
|-------|-------|---------|
| Mini-suivi (TD11-12) | 20% | UML initial + premières classes |
| Soutenance (TD15-16) | 80% | Démo complète + UML final + explications |
| Q&R (TD17-18) | pénalité possible | Questions sur le code par l'enseignant |

---

## Auteurs

Projet réalisé dans le cadre du cours de **Programmation Orientée Objet en C++**
Encadrant : **Daniel Wladdimiro** — ESILV, Second semestre 2025-2026