/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/23.
 */

#include <string_view>
#include <vector>

namespace {

const std::vector<float> cube_vertices = {
    // clang-format off
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    // clang-format on
};

constexpr std::string_view common_vsh = R"__SHADER__(
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_normal_matrix;

// NOTE(Autokaka): Output data should all based on world space here.
out vec3 v_position;
out vec3 v_normal;
out vec2 v_tex_coord;

void main() {
  gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
  v_position = vec3(u_model * vec4(a_position, 1.0));
  v_normal = mat3(u_normal_matrix) * a_normal;
  v_tex_coord = a_tex_coord;
}
)__SHADER__";

constexpr std::string_view lighted_fsh = R"__SHADER__(
#version 330 core

uniform vec3 u_view_position;

uniform struct Light {
  vec3 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
} u_light;

uniform struct Material {
  sampler2D diffuse;
  vec4 specular;
  float shininess;
} u_material;

in vec2 v_tex_coord;
in vec3 v_position;
in vec3 v_normal;

out vec4 FragColor;

vec4 GetAmbientLightColor() {
  return texture(u_material.diffuse, v_tex_coord) * u_light.ambient;
}

vec4 GetDiffuseLightColor() {
  vec3 light_direction = u_light.position - v_position;
  float diffuse = max(dot(normalize(light_direction), normalize(v_normal)), 0.0); 
  return texture(u_material.diffuse, v_tex_coord) * diffuse * u_light.diffuse;
}

vec4 GetSpecularLightColor() {
  vec3 light_direction = u_light.position - v_position;
  vec3 view_direction = normalize(u_view_position - v_position);
  vec3 reflect_direction = reflect(normalize(-light_direction), normalize(v_normal));
  float specular = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
  return u_material.specular * specular * u_light.specular;
}

void main() {
  vec4 ambient_color = GetAmbientLightColor();
  vec4 diffuse_color = GetDiffuseLightColor();
  vec4 specular_color = GetSpecularLightColor();

  FragColor = ambient_color + diffuse_color + specular_color;
}
)__SHADER__";

constexpr std::string_view lighting_fsh = R"__SHADER__(
#version 330 core

uniform vec4 u_light_color;

out vec4 FragColor;

void main() {
  FragColor = u_light_color;
}
)__SHADER__";

}  // namespace
