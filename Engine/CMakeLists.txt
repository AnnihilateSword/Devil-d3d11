cmake_minimum_required(VERSION 3.20)

project(Engine LANGUAGES CXX)

if (WIN32)
	# when a static library is generated, the Windows platform adds this code to output lib.
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# recursively find all.cpp and.h in the src directory.
file(GLOB_RECURSE EngineSrc src/*.cpp src/*.h)

# Assimp Begin --------------------------------------asdas 
# 1. Collect all Assimp files first
file(GLOB_RECURSE ExcludedFiles src/Vendor/Assimp/*.cpp src/Vendor/Assimp/*.h)

# 2. Removes excluded files from the result
list(REMOVE_ITEM EngineSrc ${ExcludedFiles})
# Assimp End --------------------------------------


        set(SHADER_TYPE "Vertex")
    elseif("${SHADER_TYPE}" STREQUAL "hs") 
        set(SHADER_TYPE "Hull")
    elseif("${SHADER_TYPE}" STREQUAL "ds") 
        set(SHADER_TYPE "Domain")
    elseif("${SHADER_TYPE}" STREQUAL "gs") 
        set(SHADER_TYPE "Geometry")
    elseif("${SHADER_TYPE}" STREQUAL "ps")
        set(SHADER_TYPE "Pixel")
    elseif("${SHADER_TYPE}" STREQUAL "cs")
        set(SHADER_TYPE "Compute")
    endif()

    set_source_files_properties(${HLSL_FILE} PROPERTIES
                                VS_SHADER_OBJECT_FILE_NAME ${PROJECT_BINARY_DIR}/shaders/hlsl/${HLSL_FBASENAME_WE}.cso
                                VS_SHADER_TYPE ${SHADER_TYPE}
                                VS_SHADER_MODEL 5.0
                                VS_SHADER_ENTRYPOINT ${ENTRY_POINT}
                                VS_SHADER_DISABLE_OPTIMIZATIONS $<$<CONFIG:Debug>:true>
                                VS_SHADER_ENABLE_DEBUG $<$<CONFIG:Debug>:true>)
endforeach()



# ************************
# generate static library.
# ************************
add_library(${PROJECT_NAME} STATIC ${EngineSrc} ${HLSL_FILES})



# compile setting.
if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE
		$<$<CONFIG:Release>:/MT>
		$<$<CONFIG:Debug>:/MTd>
	)
endif()

# add predefined.
if (WIN32)
	add_compile_definitions(
		# ...
	)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
	# ...
)

# set precompile headers.
target_precompile_headers(${PROJECT_NAME} PRIVATE 
	${CMAKE_CURRENT_LIST_DIR}/src/EnginePCH.h
)

# include library.
target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/src
	# ...
)


# *****************
# add subdirectory.
# *****************
# Assimp
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_ZLIB ON)
set(USE_STATIC_CRT ON)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/Vendor/Assimp)

# If it is a project the project is referenced first, if not, the library is linked.
# Notice that assimp is lowercase here!!!
target_link_libraries(${PROJECT_NAME} assimp)