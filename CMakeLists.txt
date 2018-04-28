cmake_minimum_required (VERSION 3.0)

add_subdirectory(external)

include_directories(
	external/cp3o/core
	external/cp3o/engine
	external/cp3o/goemetries
	external/cp3o/postprocessing
	external/cp3o/voxel

	external/glew-2.1.0/include/
	external/glm
	external/glfw/include/
	src
)

add_executable(cp3o-voxel src/main.cpp)

target_link_libraries(cp3o-voxel 
	${OPENGL_LIBRARY}
	GLEW
	glfw
	CP3O)

file(COPY ${CP3O_SHADER_DIR} DESTINATION shaders)