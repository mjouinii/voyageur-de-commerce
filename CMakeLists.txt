# 1. Version minimale de CMake
cmake_minimum_required(VERSION 3.10)

# 2. Nom du projet
project(VoyageurDeCommerce VERSION 1.0 LANGUAGES CXX)

# 3. Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Dossier include
include_directories(include)

# 5. Sources
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 6. Exécutable
add_executable(monExecutable ${SOURCES})

# 7. Options de compilation
if(MSVC)
    target_compile_options(monExecutable PRIVATE /W4)
else()
    target_compile_options(monExecutable PRIVATE -Wall -Wextra)
endif()

# 8. SFML — détection selon l'OS
if(WIN32)
    # Windows : SFML installé dans C:/SFML-2.5.1/
    set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
endif()

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(monExecutable sfml-graphics sfml-window sfml-system m)

# 9. Message de confirmation
message(STATUS "Sources détectées : ${SOURCES}")