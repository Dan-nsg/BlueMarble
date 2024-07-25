cmake_minimum_required(VERSION 3.12)
project(BlueMarble)

# Configura o executavel principal
add_executable(BlueMarble main.cpp)

# Adiciona diretorios de include
target_include_directories(BlueMarble PRIVATE
    ${CMAKE_SOURCE_DIR}/deps/glm
    ${CMAKE_SOURCE_DIR}/deps/stb
    ${CMAKE_SOURCE_DIR}/deps/glfw/include
    ${CMAKE_SOURCE_DIR}/deps/glew/include
)

# Adiciona diretorios de link
target_link_directories(BlueMarble PRIVATE
    ${CMAKE_SOURCE_DIR}/deps/glfw/lib-vc2019
    ${CMAKE_SOURCE_DIR}/deps/glew/lib/Release/x64
)

# Linka as bibliotecas necessarias
target_link_libraries(BlueMarble PRIVATE
    glfw3.lib
    glew32.lib
    opengl32.lib
)

# Copia o DLL necessario e cria um link para shaders e texturas
add_custom_command(TARGET BlueMarble POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}/glew32.dll"
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders"
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/textures" "${CMAKE_BINARY_DIR}/textures"
)

# Configura os outros executaveis
add_executable(Vectors vectors.cpp)
target_include_directories(Vectors PRIVATE ${CMAKE_SOURCE_DIR}/deps/glm)

add_executable(Matrices Matrices.cpp)
target_include_directories(Matrices PRIVATE ${CMAKE_SOURCE_DIR}/deps/glm)
