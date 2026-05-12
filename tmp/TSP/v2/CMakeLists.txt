# 1. Version minimale de CMake
cmake_minimum_required(VERSION 3.10)

# 2. Nom du projet
project(VoyageurDeCommerce VERSION 1.0 LANGUAGES CXX)

# 3. Configuration du standard C++ (équivalent de votre CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Ajout du dossier include (équivalent de -Iinclude)
include_directories(include)

# 5. Détection automatique des sources (équivalent de wildcard)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 6. Définition de l'exécutable (équivalent de monExecutable.out)
# Note : Sous Windows, CMake ajoutera automatiquement le ".exe"
add_executable(monExecutable.out ${SOURCES})

# 7. Options de compilation (équivalent de CXXFLAGS)
if(MSVC)
    # Options spécifiques à Visual Studio (Windows)
    target_compile_options(monExecutable.out PRIVATE /W4)
else()
    # Options pour GCC/Clang (Linux/MinGW)
    target_compile_options(monExecutable.out PRIVATE -Wall -Wextra)
endif()

# 8. Message de confirmation (optionnel)
message(STATUS "Sources détectées : ${SOURCES}")
