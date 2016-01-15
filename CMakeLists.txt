cmake_minimum_required(VERSION 2.8)
project(particleSim)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

add_subdirectory(lib/glfw-3.1.1)
include_directories(lib/glfw-3.1.1/include)
include_directories(lib/glew-1.13.0/include)

set(GLEW_SRC lib/glew-1.13.0/src/glew.c)

file(COPY src/colorFragmentShader.frag DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY src/colorVertexShader.vert DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY src/particlesComputeShader.comp DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(particleSim src/imageLoader.c
        src/imageLoader.h
        src/io.c
        src/io.h
        src/logic.c
        src/logic.h
        src/main.c
        src/scene.c
        src/scene.h
        src/types.h
        src/vector.c
        src/vector.h
        ${GLEW_SRC})

target_link_libraries(particleSim glfw ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})
