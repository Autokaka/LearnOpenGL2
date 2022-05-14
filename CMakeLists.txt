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
  set_target_properties(${TARGET} PROPERTIES 
    MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/res/Info.plist"
    RESOURCE "${RES}"
  )
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

# Transformations
create_exec(TransformScaleAndRotateBox "src/transformations/transform_scale_and_rotate_box.cc")
create_exec(TransformTranslateAndRotateBoxWithTime "src/transformations/transform_translate_and_rotate_box_with_time.cc")
create_exec(TransformRotateAndTranslateBoxWithTime "src/transformations/transform_rotate_and_translate_box_with_time.cc")
create_exec(Transform2DrawCall2Containers2Transform "src/transformations/transform_two_draw_call_two_containers_two_transform.cc")

# Coordinate Systems
create_exec(CoordSystemsMVPMatrix "src/coord_systems/coord_systems_mvp_matrix.cc")
create_exec(CoordSystems3DBoxWithoutDepthTest "src/coord_systems/coord_systems_3d_box_without_depth_test.cc")
create_exec(CoordSystems3DBoxWithDepthTest "src/coord_systems/coord_systems_3d_box_with_depth_test.cc")
create_exec(CoordSystemsMore3DBoxes "src/coord_systems/coord_systems_more_3d_boxes.cc")
create_exec(CoordSystemsPerspectiveProjection "src/coord_systems/coord_systems_perspective_projection.cc")
create_exec(CoordSystemsViewMatrix "src/coord_systems/coord_systems_view_matrix.cc")
create_exec(CoordSystemsRotateEvery3rdBox "src/coord_systems/coord_systems_rotate_every_3rd_box.cc")

# Camera
create_exec(CameraLookAt "src/camera/camera_look_at.cc")
create_exec(CameraMoveAround "src/camera/camera_move_around.cc")
create_exec(CameraLookAround "src/camera/camera_look_around.cc")
create_exec(CameraCameraClass "src/camera/camera_camera_class.cc")
create_exec(CameraFPSMode "src/camera/camera_fps_mode.cc")
create_exec(CameraMyOwnLookAt "src/camera/camera_my_own_look_at.cc")
