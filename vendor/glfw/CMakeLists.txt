project(glfw)

add_library(glfw INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(glfw 
  INTERFACE 
    "${PROJECT_SOURCE_DIR}/lib/libglfw3.a" 
    "-framework AppKit"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework OpenGL"
)
target_include_directories(glfw INTERFACE ${PROJECT_SOURCE_DIR}/include)
