cmake_minimum_required(VERSION 3.8.0)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glm)

function(create_exec TARGET EXEC)
  file(GLOB_RECURSE SRC "src/base/*.h" "src/base/*.cc")
  add_executable(${TARGET} ${EXEC} ${SRC})
  target_link_libraries(${TARGET} PRIVATE glad glfw glm)
  target_include_directories(${TARGET} PRIVATE ${LearnOpenGL_SOURCE_DIR}/src)
endfunction()

# Hello Window
create_exec(HelloWorld "src/hello_window/hello_window.cc")

# Hello Triangle
create_exec(HelloTriangle "src/hello_triangle/hello_triangle.cc")
create_exec(HelloTriangleExplainVaoVboEbo "src/hello_triangle/hello_triangle_explain_vao_vbo_ebo.cc")
create_exec(HelloTriangleTwoTriangles "src/hello_triangle/hello_triangle_two_triangles.cc")
create_exec(HelloTriangleTwoTrianglesTwoVaoVbo "src/hello_triangle/hello_triangle_two_triangles_two_vao_vbo.cc")
create_exec(HelloTriangleTwoTrianglesTwoShaders "src/hello_triangle/hello_triangle_two_triangles_two_shaders.cc")

# Shaders
create_exec(ShadersInOut "src/shaders/shaders_in_out.cc")
create_exec(ShadersUniform "src/shaders/shaders_uniform.cc")
create_exec(ShadersMoreAttributes "src/shaders/shaders_more_attributes.cc")
create_exec(ShadersShaderClass "src/shaders/shaders_shader_class.cc")
create_exec(ShadersTriangleUpsideDown "src/shaders/shaders_triangle_upside_down.cc")
create_exec(ShadersMovingTriangle "src/shaders/shaders_moving_triangle.cc")
create_exec(ShadersPassAPosAsFragColor "src/shaders/shaders_pass_a_pos_as_frag_color.cc")
