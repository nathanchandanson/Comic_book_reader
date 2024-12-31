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

# Lier Qt et les dépendances tierces
target_link_libraries(ComicBookReader PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
)


