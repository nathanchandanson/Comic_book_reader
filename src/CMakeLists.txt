# Créer la cible ComicBookReader
add_executable(ComicBookReader)

# Ajouter les fichiers sources
file(GLOB SOURCES
    *.cpp
    backend/*.cpp
    gui/*.cpp
    models/*.cpp
)

# Inclure les headers
include_directories(
    ${CMAKE_SOURCE_DIR}/src/backend
    ${CMAKE_SOURCE_DIR}/src/gui
    ${CMAKE_SOURCE_DIR}/src/models
)

target_sources(ComicBookReader PRIVATE ${SOURCES})

# Vérifier et lier libarchive
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBARCHIVE REQUIRED libarchive)

# Vérifier et lier Tesseract
find_package(PkgConfig REQUIRED)
pkg_check_modules(TESSERACT REQUIRED tesseract)
pkg_check_modules(LEPTONICA REQUIRED lept)

include_directories(
    ${LIBARCHIVE_INCLUDE_DIRS}
    ${TESSERACT_INCLUDE_DIRS}
    ${LEPTONICA_INCLUDE_DIRS}
)

# Lier Qt et les dépendances tierces
target_link_libraries(ComicBookReader PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    ${LIBARCHIVE_LIBRARIES}  # Lier libarchive
    ${TESSERACT_LIBRARIES}    # Lier Tesseract
    ${LEPTONICA_LIBRARIES}
)

# Inclure les répertoires d'en-têtes de libarchive
include_directories(
    ${LIBARCHIVE_INCLUDE_DIRS}
    ${Tesseract_LIBRARY_DIRS} 
)

# Optionnel : Ajouter les répertoires de bibliothèques de libarchive si nécessaire
link_directories(
    ${LIBARCHIVE_LIBRARY_DIRS}
    ${Tesseract_INCLUDE_DIRS}
)

# Créer un dossier avec toutes les données utilisateur
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/users_data)