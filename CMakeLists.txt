cmake_minimum_required (VERSION 3.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_CXX_STANDARD 11)

add_definitions(
	-DGLEW_STATIC
)

add_subdirectory(external)

include_directories(
	external/cp3o/src/core
	external/cp3o/src/engine
	external/cp3o/src/geometries
	external/cp3o/src/postprocessing
	external/cp3o/src/voxel
	external/FastNoise

	external/glew-2.1.0/include/
	external/glm
	external/glfw/include/
	src
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLEW
	glfw
	CP3O)

file(GLOB SRC_FILES 
	src/*.* 
	external/FastNoise/FastNoise.h 
	external/FastNoise/FastNoise.cpp)
add_executable(cp3o-voxel ${SRC_FILES})
target_link_libraries(cp3o-voxel ${ALL_LIBS})

add_executable(test_box test_box/main.cpp)
target_link_libraries(test_box ${ALL_LIBS})

source_group(src src)