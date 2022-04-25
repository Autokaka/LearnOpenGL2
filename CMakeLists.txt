cmake_minimum_required(VERSION 3.8.0)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw)

file(GLOB_RECURSE SRC "src/*.h" "src/*.cc")
add_executable(LearnOpenGL ${SRC})

target_link_libraries(LearnOpenGL PRIVATE glad glfw)
