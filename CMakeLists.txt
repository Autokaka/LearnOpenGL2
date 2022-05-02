cmake_minimum_required(VERSION 3.8.0)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/stb)

function(create_exec TARGET EXEC)
  file(GLOB_RECURSE RES "res/*")
  file(GLOB_RECURSE SRC "src/base/*.h" "src/base/*.cc")
  source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${EXEC} ${SRC} ${RES})

  add_executable(${TARGET} MACOSX_BUNDLE ${EXEC} ${SRC} ${RES})
  target_link_libraries(${TARGET} PRIVATE glad glfw glm stb)
  target_include_directories(${TARGET} PRIVATE ${LearnOpenGL_SOURCE_DIR}/src)
  set_target_properties(${TARGET} PROPERTIES RESOURCE "${RES}")
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

# Textures
create_exec(TexturesLoadTexture "src/textures/textures_load_texture.cc")
create_exec(TexturesTextureMixColor "src/textures/textures_texture_mix_color.cc")
create_exec(TexturesMixTextures "src/textures/textures_mix_textures.cc")
create_exec(TexturesHappyFaceHFlipped "src/textures/textures_happy_face_h_flipped.cc")
create_exec(TexturesTextureWrapping "src/textures/textures_texture_wrapping.cc")
create_exec(TexturesOnlyViewPart "src/textures/textures_only_view_part.cc")
create_exec(TexturesOnlyViewPartNearestFiltering "src/textures/textures_only_view_part_nearest_filtering.cc")
create_exec(TexturesUniformControlMix "src/textures/textures_uniform_control_mix.cc")
