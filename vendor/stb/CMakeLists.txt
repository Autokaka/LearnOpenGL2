project(stb)

file(GLOB_RECURSE SRC "*" "**/*")

add_library(stb INTERFACE ${SRC})
target_include_directories(stb INTERFACE include)
