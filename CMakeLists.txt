cmake_minimum_required(VERSION 4.0)
project(simulator)

set(CMAKE_CXX_STANDARD 23)

add_library(imgui STATIC
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp)
target_include_directories(imgui INTERFACE imgui)

add_library(sokol STATIC
	sokol/sokol.cxx
	sokol/sokol_app.h
	sokol/sokol_gfx.h
	sokol/sokol_glue.h
	sokol/sokol_log.h)
target_link_libraries(sokol PRIVATE GL dl m)
target_include_directories(sokol INTERFACE sokol)

target_link_libraries(sokol PUBLIC imgui)

add_executable(simulator
	src/main.cxx)

target_link_libraries(simulator sokol)

set_target_properties(simulator PROPERTIES
	OUTPUT_NAME "index"
	SUFFIX ".html"
)
target_link_options(simulator PRIVATE --shell-file ../web/index.html)
target_link_options(simulator PRIVATE -sUSE_WEBGL2=1)
