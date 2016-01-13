cmake_minimum_required(VERSION 3.2)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW glfw3)

add_executable(particles src/imageLoader.c
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
	src/vector.h)

target_link_libraries(particles ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})