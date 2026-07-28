cmake_minimum_required(VERSION 3.20)
project(GravitySimulator C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glm)

set(SRC_FILES
  src/main.cpp
  src/DrawEngine.h
  src/DrawEngine.cpp
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    libs/glad/src/glad.c
)
target_include_directories(${PROJECT_NAME} PRIVATE
    libs/glad/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    opengl32
    glm::glm
)
