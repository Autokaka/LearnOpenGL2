project(glm)

file(GLOB_RECURSE SRC "*" "**/*")

add_library(glm INTERFACE ${SRC})
target_include_directories(glm INTERFACE include)
target_compile_options(glm INTERFACE -DGLM_FORCE_CXX2A)
