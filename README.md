# Voyageur de Commerce - TSP

Description du projet ...

![Capture d'écran du projet](lien-vers-image.png)

## Architecture du projet :
backups/ -> archive.tar.gz (sauvegarde du projet)

instances/ -> fichier.tsp (données à traiter)

include/ -> fichier.h (définition de structures et de fonctions)

build/ -> fichier.o (objet - binaire)

log/ -> fichier.log (journalisation de l'état du programme)

fichier.out (exécutable)

res/ (resources)

src/ -> fichier.cpp 

## Utilisation de git

À faire avant de git pull pour vérifier si les différences entre son code et le dépot :
`git status`

Pour ajouter les modifications du dépot sans toucher à son propre code :
`git fetch origin`

## 🧰 Makefile pour Linux

Compiler le projet : `make`

Nettoyer les fichiers objets : `make clean`

Faire un backup du projet : `make backup`

## CMakeLists.txt pour Windows

Génère un Makefile compatible Windows (MinGW) tout en continuant de fonctionner pour Linux.

`mkdir build`
`cd build`
`cmake ..`
`cmake --build .`

## 🛠 Stack Technique
**C++**

## 🤝 Contributeur 
Jouini Moslem & Alexis Kerboul