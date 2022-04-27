cmake_minimum_required(VERSION 3.8.0)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/utils.cmake)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw)

# Hello Window
create_exec(HelloWorld "src/hello_window.cc")

# Hello Triangle
create_exec(HelloTriangle "src/hello_triangle/hello_triangle.cc")
create_exec(HelloTriangleExplainVaoVboEbo "src/hello_triangle/hello_triangle_explain_vao_vbo_ebo.cc")
create_exec(HelloTriangleTwoTriangles "src/hello_triangle/hello_triangle_two_triangles.cc")
create_exec(HelloTriangleTwoTrianglesTwoVaoVbo "src/hello_triangle/hello_triangle_two_triangles_two_vao_vbo.cc")
create_exec(HelloTriangleTwoTrianglesTwoShaders "src/hello_triangle/hello_triangle_two_triangles_two_shaders.cc")
