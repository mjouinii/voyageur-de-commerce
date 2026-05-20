# Voyageur de Commerce — TSP

Implémentation du problème du Voyageur de Commerce (TSP) en C++.
Le programme trouve le chemin le plus court passant par toutes les villes
en utilisant deux algorithmes : Nearest Neighbor + 2-opt.

## 🗺️ Ce que fait le programme

1. Lit un fichier `.tsp` au format TSPLIB
2. Lance l'algorithme **Nearest Neighbor** depuis chaque ville de départ
3. Améliore le meilleur chemin trouvé avec le **2-opt**
4. Affiche le chemin et la distance dans le terminal
5. Ouvre une fenêtre graphique (SFML) avec le chemin tracé

## 📁 Architecture du projet

```
voyageur-de-commerce/
├── include/          # Fichiers .h (déclarations)
├── src/              # Fichiers .cpp (implémentation)
├── instances/        # Fichiers .tsp (données)
├── build/            # Fichiers objets .o (générés à la compilation)
├── backups/          # Archives de sauvegarde
├── Makefile          # Pour Linux/Mac
├── CMakeLists.txt    # Pour Windows
└── README.md
```

## ⚙️ Dépendances

- **g++** (GCC ≥ 6.0)
- **SFML 2.5.1** — bibliothèque graphique

### Installer SFML sur Linux
```bash
sudo apt install libsfml-dev
```

### Installer SFML sur Windows
Télécharger SFML 2.5.1 pour MinGW depuis :
https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-32-bit.zip

Extraire dans `C:/SFML-2.5.1/`

## 🔧 Compilation

### Linux — avec Makefile
```bash
make
```

### Windows — avec Makefile (Git Bash + MinGW)
```bash
make
```

### Windows — avec CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Nettoyer les fichiers compilés
```bash
make clean
```

## 🚀 Exécution

```bash
./monExecutable.out <fichier.tsp>
```

Exemples :
```bash
./monExecutable.out bayg29.tsp
./monExecutable.out att48.tsp
./monExecutable.out gr24.tsp
./monExecutable.out ch150.tsp
```

Les fichiers `.tsp` doivent être dans le dossier `instances/`.

## 📊 Instances disponibles

| Fichier | Villes | Format |
|---|---|---|
| gr24.tsp | 24 | EXPLICIT UPPER_ROW |
| bayg29.tsp | 29 | EXPLICIT UPPER_ROW |
| att48.tsp | 48 | ATT (coordonnées) |
| ch150.tsp | 150 | ATT (coordonnées) |

## 🧠 Algorithmes

**Nearest Neighbor** — heuristique gloutonne : à chaque étape, aller à la ville non visitée la plus proche. Lancé depuis chaque ville de départ, on garde le meilleur résultat.

**2-opt** — amélioration locale : inverse des segments du chemin pour supprimer les croisements. Répété jusqu'à ce qu'aucune amélioration ne soit possible.

## 🛠 Stack Technique

**C++** — compilé avec g++ (standard C++17)

## 🤝 Contributeurs

Jouini Moslem & Alexis Kerboul