# ──────────────────────────────────────────────────────────────
# Makefile — Voyageur de Commerce (TSP)
# Compatible Windows (MinGW) et Linux
# ──────────────────────────────────────────────────────────────

# Compilateur
CXX      = g++

# Options de compilation
# 	-Wall    → affiche tous les avertissements
# 	-Iinclude → ajoute le dossier include aux chemins de recherche
CXXFLAGS = -Wall -Iinclude

# Détection automatique de l'OS
# 	Sur Windows, la variable OS vaut "Windows_NT"
# 	Sur Linux/Mac, elle est absente
ifeq ($(OS), Windows_NT)
    # Windows : SFML installé dans C:/SFML-2.5.1/
    CXXFLAGS += -IC:/SFML-2.5.1/include
    LDFLAGS   = -lm -LC:/SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system
    TARGET    = monExecutable.exe
else
    # Linux : SFML installé via apt (sudo apt install libsfml-dev)
    LDFLAGS   = -lm -lsfml-graphics -lsfml-window -lsfml-system
    TARGET    = monExecutable.out
endif

# Liste tous les fichiers .cpp dans src/
# 	'wildcard' → scanne le disque et retourne tous les fichiers correspondants
SOURCES = $(wildcard src/*.cpp)

# Transforme la liste des .cpp en liste de .o dans build/
# 	'patsubst' → remplace src/ par build/ et .cpp par .o
OBJECTS = $(patsubst src/%.cpp, build/%.o, $(SOURCES))

# ── Règle principale : créer l'exécutable ─────────────────────
$(TARGET) : $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# ── Règle générique : compiler chaque .cpp en .o ──────────────
# 	@mkdir -p build → crée le dossier build si nécessaire (@ = silencieux)
# 	$<             → la première dépendance (le fichier .cpp)
# 	$@             → la cible (le fichier .o)
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ── Nettoyage ─────────────────────────────────────────────────
clean:
	rm -rf build $(TARGET)

# ── Sauvegarde ────────────────────────────────────────────────
backup:
	./backup.sh