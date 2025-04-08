cmake_minimum_required(VERSION 3.26.0)
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_NAME}
 VERSION 1.0.0
 DESCRIPTION "fumo_engine"
 LANGUAGES C CXX
)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
##########################################################################################
# Configurable options
##########################################################################################
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
        "master"
    )

    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/external/raylib/config.h ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/raylib-src/src/config.h COPYONLY)
        FetchContent_MakeAvailable(${dep})
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
# Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
##########################################################################################
# Compiler and linker flags
##########################################################################################


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3 -DDEBUG -fsanitize=address -Werror -g -march=native -pthread")
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

##########################################################################################
# Build flags and additional settings
##########################################################################################

# copy assets over into the build dir for the executable to have them for debug/run reasons
add_custom_target(copyAssetsOver
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory"
)
add_dependencies(${PROJECT_NAME} copyAssetsOver)

add_definitions( -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} )
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")


set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
