project(stb)

file(GLOB_RECURSE SRC "*.h" "**/*.h" "*.cc" "**/*.cc")

add_library(stb STATIC ${SRC})
target_include_directories(stb PUBLIC include)
