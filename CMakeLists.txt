cmake_minimum_required(VERSION 3.26.0)
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_NAME}
	VERSION 1.0.0
	DESCRIPTION "solar system gamign"
	# HOMEPAGE_URL "none"
	LANGUAGES C CXX
	# CMAKE_BUILD_TYPE Debug
)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
##########################################################################################
# Configurable options
##########################################################################################
# set(CXX_SCAN_FOR_MODULES ON)
# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED YES)
# set(CMAKE_CXX_EXTENSIONS OFF)
# toggle libs to be used with ON and OFF
option(USE_NO_ASSETS "Use no assets" OFF)
option(USE_RAYLIB "Use raylib" ON)
option(USE_LIBASSERT "Use libassert" ON)

# libraries array to be linked
set(LIBRARIES ${PROJECT_NAME})

include(FetchContent)
function(add_git_dependency libName gitURL gitTag)

        FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG        ${gitTag}
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
        )

        if(NOT ("${libName}" STREQUAL "libname"))
            FetchContent_MakeAvailable(${libName})
        endif()

        # exclude lib to avoid errors
        # if(NOT "${libName}" STREQUAL "libname" AND NOT "${libName}" STREQUAL "rlImGui")
            # target_compile_options(${libName} PRIVATE "-w")
        # endif()  
endfunction()

#[===[
    NOTE: if you want to get the latest commit to a repository
    git rev-parse HEAD
    copy the commit hash if you want a specific commit
]===]

if(USE_LIBASSERT)
    set(dep libassert)
    add_git_dependency(
        ${dep}
        https://github.com/jeremy-rifkin/libassert.git
        v2.1.5
    )
    include_directories(RECURSE build/_deps/libassert-src/src)
    list(APPEND LIBRARIES libassert::assert)
endif()

##########################################################################################
# Raylib Setup
##########################################################################################

if(USE_RAYLIB)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(CUSTOMIZE_BUILD OFF CACHE BOOL "" FORCE) 

    set(dep raylib)
    add_git_dependency(
        ${dep}
        https://github.com/raysan5/raylib.git
        HEAD
    )
    
    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/external/raylib/config.h ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/raylib-src/src/config.h COPYONLY)
        FetchContent_Populate(${dep})
        
        add_subdirectory(${${dep}_SOURCE_DIR} ${${dep}_BINARY_DIR})
    endif()
        
    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_GLFW_WAYLAND")
endif()
# GLFW_LINUX_ENABLE_WAYLAND = TRUE
# GLFW_LINUX_ENABLE_X11 = FALSE

##########################################################################################
# Adding our source files
# Define PROJECT_SOURCES as a list of all source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
# file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.pcm")
# Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Declaring our executable
add_executable(${PROJECT_NAME})
# add_dependencies(${PROJECT_NAME} std.compat)
# target_link_libraries( ${PROJECT_NAME} std.compat)
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
# target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
##########################################################################################
# Compiler and linker flags
##########################################################################################


    # if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    #     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libstdc++ -static-libgcc -static-libstdc++ -pthread")
        # target_link_libraries(${LIBRARIES} -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
        # dont forget to set  "cmake.configureSettings": { "CMAKE_BUILD_TYPE": "${buildType}" } in settings.json for vs code
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3 -DDEBUG -fsanitize=address -Werror -g -march=native -pthread")
            # SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fmodule-file=std=std.compat.pcm -fmodule-file=std=std.pcm")

        # else()
    # SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3 -g -march=native")
    target_link_libraries(${LIBRARIES})
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(${LIBRARIES} -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    target_link_libraries(${LIBRARIES} -static-intel -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(${LIBRARIES})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")  
endif()
    # endif()

##########################################################################################
# Build flags and additional settings
##########################################################################################

# copy assets over into the build dir for the executable to have them for debug/run reasons
# if(NOT USE_NO_ASSETS)
add_custom_target(copyAssetsOver
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory"
)
add_dependencies(${PROJECT_NAME} copyAssetsOver) 
# endif()

add_definitions( -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} )
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")   


##########################################################################################
##########################################################################################
# Setting ASSETS_PATH
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    # message("-- console enabled")
    if(NOT WIN32)
        SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -DDEBUG -fsanitize=address -Werror -g -march=native")
    endif()
        # Set the asset path macro to the absolute path on the dev machine
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fsanitize=address")
        # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable
        # target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/")
    if(WIN32)
      if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows") 
      endif()    
      add_compile_definitions(NDEBUG)
    endif()
endif()
if(NOT WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable")
endif()
# Set common compiler flags
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
# set_property(TARGET ${PROJECT_NAME} PROPERTY CMAKE_C_STANDARD 99)
# if (${PLATFORM} STREQUAL "Web")
#     # Tell Emscripten to build an .html file.
#     set(CMAKE_EXECUTABLE_SUFFIX ".html")
#     SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Os")
#     set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
#     set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
#     # if(NOT USE_NO_ASSETS)
#         # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -Os -Wall -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --preload-file assets/ --shell-file ../src/minshell.html")
#         set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -Os -Wall -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --preload-file assets/ --shell-file ../vendor/minshell.html")
#         target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="/assets/") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable
#     # else()
#     #     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -Os -Wall -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --shell-file ../vendor/minshell.html")
#     endif()
# endif()
#
