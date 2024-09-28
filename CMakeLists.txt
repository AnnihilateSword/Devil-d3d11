cmake_minimum_required(VERSION 3.20)

project(Devil LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# setup visual studio start project.
if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ProvingGround)
endif()

add_subdirectory(Engine)
add_subdirectory(ProvingGround)