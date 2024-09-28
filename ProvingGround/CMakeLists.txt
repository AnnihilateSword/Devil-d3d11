cmake_minimum_required(VERSION 3.20)

project(ProvingGround LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# recursively find all.cpp and.h in the src directory.
file(GLOB_RECURSE ProvingGroundSrc src/*.cpp src/*.h)

# Set the generated executable file to be a Windows application.
if (WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
endif()



# ************************
# set vs working directory
# ************************
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
	$<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_BINARY_DIR}/Debug,${CMAKE_CURRENT_BINARY_DIR}/Release>
)



# *************************
# generate executable file.
# *************************
add_executable(${PROJECT_NAME} ${ProvingGroundSrc}
    # generate ico for .exe file
    ${CMAKE_SOURCE_DIR}/Engine/Engine.rc
)

# compile setting.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/MT>
        $<$<CONFIG:Debug>:/MTd>
    )
endif()



# ****************
# include library.
# ****************
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine/src
)

# If it is a project, the project is referenced first, if not, the library is linked.
target_link_libraries(${PROJECT_NAME} Engine)



# *************************************
# 检查 Debug/Release 模式下的文件是否存在
# 复制指定 文件 或 文件夹
# *************************************
if (WIN32)

    if (EXISTS ${CMAKE_SOURCE_DIR}/Engine/shaders)
        # Engine.lib and shaders/
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            # Command1
            COMMAND ${CMAKE_COMMAND} -E copy 
            $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/Engine/Debug/Engine.lib,${CMAKE_SOURCE_DIR}/Engine/Release/Engine.lib>
            $<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_BINARY_DIR}/Debug/Engine.lib,${CMAKE_CURRENT_BINARY_DIR}/Release/Engine.lib>

            # Log1: Message for Engine.lib
            COMMAND ${CMAKE_COMMAND} -E echo "----[Devil Engine CMake Log]----: Copying **( Engine.lib )** based on build configuration!!!"

            #**************************************** shaders/
            # Command2
            COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/Engine/shaders
            $<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_BINARY_DIR}/Debug/shaders,${CMAKE_CURRENT_BINARY_DIR}/Release/shaders>

            # Log2: Message for shaders/
            COMMAND ${CMAKE_COMMAND} -E echo "----[Devil Engine CMake Log]----: Copying **( shaders/ )** based on build configuration!!!"

            # Command3 (using for ide test)
            COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/Engine/shaders
            ${CMAKE_CURRENT_BINARY_DIR}/shaders

            # Log3: Message for shaders/
            COMMAND ${CMAKE_COMMAND} -E echo "----[Devil Engine CMake Log]----: Copying **( shaders/ )** based on build configuration!!!"

            #**************************************** content/
            # Command4
            COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/Engine/content
            $<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_BINARY_DIR}/Debug/content,${CMAKE_CURRENT_BINARY_DIR}/Release/content>

            # Log2: Message for shaders/
            COMMAND ${CMAKE_COMMAND} -E echo "----[Devil Engine CMake Log]----: Copying **( content/ )** based on build configuration!!!"

            # Command5 (using for ide test)
            COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/Engine/content
            ${CMAKE_CURRENT_BINARY_DIR}/content

            # Log3: Message for shaders/
            COMMAND ${CMAKE_COMMAND} -E echo "----[Devil Engine CMake Log]----: Copying **( content/ )** based on build configuration!!!"
        )
    else()
        # Engine.lib
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            # Command1
            COMMAND ${CMAKE_COMMAND} -E copy 
            $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/Engine/Debug/Engine.lib,${CMAKE_SOURCE_DIR}/Engine/Release/Engine.lib>
            $<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_BINARY_DIR}/Debug/Engine.lib,${CMAKE_CURRENT_BINARY_DIR}/Release/Engine.lib>

            # Log1: Message for Engine.lib
            COMMAND ${CMAKE_COMMAND} -E echo "----[Devil Engine CMake Log]----: Copying **( Engine.lib )** based on build configuration!!!"
        )
    endif()


endif()