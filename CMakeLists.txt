cmake_minimum_required(VERSION 3.16)
project(ComicBookReader)

# Configuration de C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Spécifier l'endroit où est installé Qt
set(CMAKE_PREFIX_PATH "/home/nchan/Qt/6.8.1/gcc_64/lib/cmake")
# Trouver Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
# Activer AUTOMOC pour générer automatiquement le code moc
set(CMAKE_AUTOMOC ON)

# Ajouter les sous-dossiers
add_subdirectory(src)

# Copier les ressources (icônes, thèmes) dans le dossier de build
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
)
