# Variables de configuration
# 	"-Wall" -> Warning All
# 	"-Iinclude" - permet d'ajouter le dossier include à la liste des chemins
#	 de recherche prioritaires
CXX = g++ 
CXXFLAGS = -Wall -Iinclude

# Liste tous les fichiers finissant par .cpp dans src
# 	'wildcard' -> caractère génériques - est une fonction qui scanne le disque
SOURCES = $(wildcard src/*.cpp)

# Transforme la liste des .cpp en liste de .o dans build/
# 	'Path Substitution - est une fonction qui transforme du texte
# 	Prend la liste SOURCES et, pour chaque fichier, remplace le 
# 	début src/ par build/ et la fin .cpp par .o
OBJECTS = $(patsubst src/%.cpp, build/%.o, $(SOURCES))

# Création de l'exécutable
monExecutable.out : $(OBJECTS)
	$(CXX) $(OBJECTS) -o monExecutable.out

# Règle Générique
# 	"build/%.o: src/%.cpp" fabrique n'importe quel fichier .o dans
# 	build, cherhce le fichier .cpp correspondant dans src
#
# 	"@mkdir -p build" crée le dossier build s'il n'existe pas. Le '@' rend la commande
# 	'muette' (elle ne s'affiche pas dans le terminal)
#
# 	"$<" est une variable automatique qui signifie 'la première dépendance' 
# 	(le fichier .cpp)
#
# 	"$@" est une variable automatique qui signifie 'la cible' (le fichier .o) 
build/%.o: src/%.cpp
	@mkdir -p build 
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf build monExecutable.out

# Backup
backup:
	./backup.sh