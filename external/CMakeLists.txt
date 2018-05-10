set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)

add_definitions(
	-DGLEW_STATIC
)

### GLFW ###

add_subdirectory (glfw)

include_directories(glfw/include/GLFW)
include_directories(glew-2.1.0/include)

### GLEW ###

set(GLEW_SOURCE
	glew-2.1.0/src/glew.c
)

set(GLEW_HEADERS
)

add_library(GLEW STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW
	${OPENGL_LIBRARY}
)

### cp3o

add_subdirectory(cp3o)