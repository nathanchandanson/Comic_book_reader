# Installation 
## Dépendances
Installer toutes les dépendances nécessaires à Qt6 et à la compilation
```bash
sudo apt-get updates
sudo apt install build-essential cmake libgl1-mesa-dev libxkbcommon-x11-0 libxcb-icccm libxcb-image0 libxcb-keysyms1 libxcb-render-util0 libxcb-shape0 libxcb-cursor0 libxcb-cursor-dev libarchive-dev pkg-config
```

## Installer Qt
Il faut un compte Qt. Ensuite, récupérer le fichier d'installation 
```bash
wget https://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run
chmod +x qt-unified-linux-x64-online.run
```

Ensuite, lancer l'installateur et suivre les étapes
```bash
./qt-unified-linux-x64-online.run
```

# Compilation
## Initialisation répertoire build
Se mettre dans le répertoire build
```bash
cd build
```

Lancer la commande 
```bash
cmake ..
```

## Compilation 
Pour compiler, on se met dans le dossier build, puis il faut créer le makefile avec
```bash
cmake --build
```

Ensuite, on peut se rendre dans le répertoire build/src qui contiendra le makefile à utiliser, ainsi que le fichier executable généré.
On fera alors, pour compiler puis pour executer 
```bash
make
./ComicBookReader
```


# Architecture
BDReader/
│
├── build/                  # Fichiers de build générés (CMake ou autres)
│
├── data/                   # Ressources statiques ou fichiers d'exemple
│   ├── comics/             # Fichiers de BD d'exemple (.cbr, .cbz, etc.)
│   └── icons/              # Icônes pour l'interface utilisateur
│
├── doc/                    # Documentation
│   ├── design/             # Diagrammes UML, mockups de l'interface
│   └── README.md           # Guide d'utilisation ou notes de projet
│
├── src/                    # Code source principal
│   ├── backend/            # Gestion des fichiers .cbr/.cbz et extraction des données
│   │   ├── Archive.cpp
│   │   ├── Archive.hpp
<!-- │   │   ├── Image.cpp
│   │   ├── Image.hpp
│   │   ├── MetadataExtractor.cpp
│   │   └── MetadataExtractor.hpp -->
│   │
│   ├── gui/                # Interface utilisateur avec Qt
│   │   ├── MainWindow.cpp
│   │   └── MainWindow.hpp
│   │
│   ├── models/             # Structures de données et modèles pour les BD
│   │   ├── ComicBook.cpp
│   │   └── ComicBook.hpp
│   │
<!-- │   ├── utils/              # Fonctions utilitaires génériques
│   │   ├── FileUtils.cpp
│   │   ├── FileUtils.hpp
│   │   ├── ImageUtils.cpp
│   │   └── ImageUtils.hpp -->
│   │
│   ├── main.cpp            # Point d'entrée du programme
│   └── CMakeLists.txt      # Configuration CMake pour ce dossier
│
├── .gitignore              # Fichiers à ignorer par Git
├── CMakeLists.txt          # Configuration CMake principale
└── README.md               # Documentation principale du projet
