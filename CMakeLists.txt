cmake_minimum_required(VERSION 2.6)

# name of the project
set(PROJ_NAME COMP3009-Final-Project)
project(${PROJ_NAME})

# set include, src, and resource directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/resource)
include_directories(${PROJECT_SOURCE_DIR}/imgui)
include_directories(${PROJECT_SOURCE_DIR}/imgui/backends)

# specify the project source files and add the executable
set(SRCS
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/Camera.cpp
	${PROJECT_SOURCE_DIR}/src/App.cpp
	${PROJECT_SOURCE_DIR}/src/World.cpp
	${PROJECT_SOURCE_DIR}/src/Shader.cpp
	${PROJECT_SOURCE_DIR}/src/Noise.cpp
	${PROJECT_SOURCE_DIR}/src/Texture.cpp
	${PROJECT_SOURCE_DIR}/src/GUI.cpp

	${PROJECT_SOURCE_DIR}/include/Camera.h
	${PROJECT_SOURCE_DIR}/include/App.h
	${PROJECT_SOURCE_DIR}/include/World.h
	${PROJECT_SOURCE_DIR}/include/Shader.h
	${PROJECT_SOURCE_DIR}/include/Noise.h
	${PROJECT_SOURCE_DIR}/include/Texture.h
	${PROJECT_SOURCE_DIR}/include/GUI.h

	${PROJECT_SOURCE_DIR}/resource/shader.vert
	${PROJECT_SOURCE_DIR}/resource/shader.frag

	${PROJECT_SOURCE_DIR}/imgui/imconfig.h
	${PROJECT_SOURCE_DIR}/imgui/imgui.h
	${PROJECT_SOURCE_DIR}/imgui/imgui_internal.h
	${PROJECT_SOURCE_DIR}/imgui/imstb_rectpack.h
	${PROJECT_SOURCE_DIR}/imgui/imstb_textedit.h
	${PROJECT_SOURCE_DIR}/imgui/imstb_truetype.h
	${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.h
	${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.h

	${PROJECT_SOURCE_DIR}/imgui/imgui.cpp
	${PROJECT_SOURCE_DIR}/imgui/imgui_demo.cpp
	${PROJECT_SOURCE_DIR}/imgui/imgui_draw.cpp
	${PROJECT_SOURCE_DIR}/imgui/imgui_tables.cpp
	${PROJECT_SOURCE_DIR}/imgui/imgui_widgets.cpp
	${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
	${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
)
add_executable(${PROJ_NAME} ${SRCS})

# require OpenGL libraries
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

# set library path
set(LIBRARY_PATH C:/Users/benwi/Documents/University/Third\ Year/COMP3009A\ -\ Computer\ Graphics/libraries)
include_directories(${LIBRARY_PATH}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/noise)

#find libraries for linux/Mac OS users
if(NOT WIN32)
	find_library(GLEW_LIBRARY GLEW)
	find_library(GLFW_LIBRARY GLFW)
	find_library(SOIL_LIBRARY SOIL)
	find_library(GLUT_LIBRARY GLUT)
# find libraries for windows users
elseif(WIN32)
	find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
	find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
	find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
	find_library(GLUT_LIBRARY freeglut HINTS ${LIBRARY_PATH}/lib)

	file(COPY ${LIBRARY_PATH}/lib/freeglut.dll
         DESTINATION ${CMAKE_BINARY_DIR})
endif(NOT WIN32)

target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLUT_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})

# use the proper libraries in debug mode
if(WIN32)
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)