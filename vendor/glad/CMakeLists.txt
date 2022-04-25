project(glad)

file(GLOB_RECURSE SRC "*.h" "*.c")
add_library(glad STATIC ${SRC})
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/include)
