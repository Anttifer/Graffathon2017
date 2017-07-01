cmake_minimum_required(VERSION 2.8)
project(impromptu)

file(GLOB_RECURSE SRC_FILES src/*)
file(GLOB_RECURSE INCLUDE_FILES include/*)

include_directories(include)

find_package(Eigen3 REQUIRED)
find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(impromptu
	${SRC_FILES}
	${INCLUDE_FILES}
)

target_include_directories(impromptu
	PRIVATE
	${GLEW_INCLUDE_DIRS}
	${GLFW3_INCLUDE_DIRS}
	${EIGEN3_INCLUDE_DIRS}
)

target_link_libraries(impromptu
	PRIVATE
	glfw
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
)

set_property(TARGET impromptu PROPERTY CXX_STANDARD 14)
