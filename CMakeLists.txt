cmake_minimum_required(VERSION 3.16)
project(hello)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the output directory for built objects.
# This make sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendors/SDL EXCLUDE_FROM_ALL)

include_directories(${CMAKE_SOURCE_DIR}/vendors/imgui)

file(GLOB SOURCES "src/*.cpp")

file(GLOB IMGUI_SOURCES
	vendors/imgui/imgui.cpp
	vendors/imgui/imgui.demo.cpp
	vendors/imgui/imgui_draw.cpp
	vendors/imgui/imgui_tables.cpp
	vendors/imgui/imgui_widgets.cpp
	vendors/imgui/backends/imgui_impl_sdl3.cpp
	vendors/imgui/backends/imgui_impl_vulkan.cpp
)

# Create your game executable target as usual
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})

target_include_directories(${PROJECT_NAME}
	PRIVATE
	${PROJECT_SOUCE_DIR}/include
)

find_package(Vulkan REQUIRED)

# Link to the actual SDL3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 Vulkan::Vulkan)
