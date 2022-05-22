// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <imgui/imgui.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

#include "base/app.h"
#include "base/buffer.h"
#include "base/camera.h"
#include "base/device.h"
#include "base/shader.h"

class MyApp final : public App, AppDelegate {
 public:
  static SharedApp GetInstance() {
    if (!singleton_) {
      singleton_ = SharedApp(new MyApp());
    }
    return singleton_;
  }

  void AppThrowsError(const std::string& error) override {
    std::cout << error << std::endl;
    Exit(-1);
  }

  void WindowSizeDidChange(int width, int height) override {
    glViewport(0, 0, width, height);
  }

  void KeyboardEvent(const KeyStateGetter& key_state_getter) override {
    if (key_state_getter(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      Exit(0);
    }

    if (key_state_getter(GLFW_KEY_UP) == GLFW_PRESS) {
      camera_.GoForward(0.1f);
    }
    if (key_state_getter(GLFW_KEY_DOWN) == GLFW_PRESS) {
      camera_.GoBackward(0.1f);
    }
    if (key_state_getter(GLFW_KEY_LEFT) == GLFW_PRESS) {
      camera_.GoLeft(0.1f);
    }
    if (key_state_getter(GLFW_KEY_RIGHT) == GLFW_PRESS) {
      camera_.GoRight(0.1f);
    }

    if (key_state_getter(GLFW_KEY_W) == GLFW_PRESS) {
      camera_.PitchUp(1);
    }
    if (key_state_getter(GLFW_KEY_S) == GLFW_PRESS) {
      camera_.PitchDown(1);
    }
    if (key_state_getter(GLFW_KEY_A) == GLFW_PRESS) {
      camera_.YawLeft(1);
    }
    if (key_state_getter(GLFW_KEY_D) == GLFW_PRESS) {
      camera_.YawRight(1);
    }
  }

  void WindowWillAppear() override {
    // create VBO
    // clang-format off
    SharedVertexBuffer vbo = VertexBuffer::Create(std::vector<float>({
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    }), std::vector<Attribute>({
      { "a_position", AttributeType::kVec3 },
      { "a_normal", AttributeType::kVec3 },
    }));
    // clang-format on
    device_->UseVertexBuffer(vbo);

    // create shader program
    const char* common_vsh = R"__SHADER__(
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_normal_matrix;

// NOTE(Autokaka): Output data should all based on view space.
out vec3 v_position;
out vec3 v_normal;

void main() {
  gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
  v_position = vec3(u_view * u_model * vec4(a_position, 1.0));
  v_normal = vec3(u_view * u_normal_matrix * vec4(a_normal, 0.0));
}
)__SHADER__";
    const char* lighted_fsh = R"__SHADER__(
#version 330 core

// NOTE(Autokaka): Uniform data should all based on view space.
uniform vec4 u_entity_color;
uniform vec4 u_light_color;
uniform vec3 u_light_position;
uniform vec3 u_view_position;

uniform float u_ambient;
uniform float u_diffuse;
uniform float u_specular;
uniform float u_shininess;

// NOTE(Autokaka): Input data should all based on view space.
in vec3 v_position;
in vec3 v_normal;

out vec4 FragColor;

vec4 GetAmbientLightColor() {
  return u_ambient * u_light_color;
}

vec4 GetDiffuseLightColor() {
  vec3 light_direction = u_light_position - v_position;
  float diffuse = max(dot(normalize(light_direction), normalize(v_normal)), 0.0); 
  return u_diffuse * diffuse * u_light_color;
}

vec4 GetSpecularLightColor() {
  vec3 light_direction = u_light_position - v_position;
  vec3 view_direction = normalize(u_view_position - v_position);
  vec3 reflect_direction = reflect(normalize(-light_direction), normalize(v_normal));
  float specular = pow(max(dot(view_direction, reflect_direction), 0.0), u_shininess);
  return u_specular * specular * u_light_color;
}

void main() {
  vec4 ambient_color = GetAmbientLightColor();
  vec4 diffuse_color = GetDiffuseLightColor();
  vec4 specular_color = GetSpecularLightColor();

  vec4 final_light_color = ambient_color + diffuse_color + specular_color;
  FragColor = final_light_color * u_entity_color;
}
)__SHADER__";
    const char* lighting_fsh = R"__SHADER__(
#version 330 core

uniform vec4 u_light_color;

out vec4 FragColor;

void main() {
  FragColor = u_light_color;
}
)__SHADER__";
    lighted_shader_ = Shader::CreateFromSource(common_vsh, lighted_fsh);
    lighting_shader_ = Shader::CreateFromSource(common_vsh, lighting_fsh);
    if (!lighted_shader_ || !lighting_shader_) {
      Exit(-1);
    }

    camera_.SetPosition(glm::vec3(2, 2, 3));
    camera_.LookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
  }

  void DrawContent() override {
    // do actual rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 view = glm::inverse(camera_.GetTransformMatrix());
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.f);

    {  // draw lighted entity
      device_->UseProgram(lighted_shader_);
      {
        lighted_shader_->SetVec4("u_entity_color", entity_color_);
        lighted_shader_->SetVec4("u_light_color", light_color_);
        lighted_shader_->SetVec3("u_light_position",
                                 view * glm::vec4(light_position_, 1));
        lighted_shader_->SetVec3("u_view_position", glm::vec3(0, 0, 0));

        lighted_shader_->SetFloat("u_ambient", ambient_);
        lighted_shader_->SetFloat("u_diffuse", diffuse_);
        lighted_shader_->SetFloat("u_specular", specular_);
        lighted_shader_->SetFloat("u_shininess", shininess_);
      }
      {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
        lighted_shader_->SetMat4("u_model", model);
        lighted_shader_->SetMat4("u_view", view);
        lighted_shader_->SetMat4("u_projection", projection);
        lighted_shader_->SetMat4("u_normal_matrix", normal_matrix);
      }
      device_->DrawContent();
    }

    {  // draw lighting entity
      device_->UseProgram(lighting_shader_);
      { lighting_shader_->SetVec4("u_light_color", light_color_); }
      {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), light_position_);
        model = glm::scale(model, glm::vec3(0.1f));
        glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
        lighting_shader_->SetMat4("u_model", model);
        lighting_shader_->SetMat4("u_view", view);
        lighting_shader_->SetMat4("u_projection", projection);
        lighting_shader_->SetMat4("u_normal_matrix", normal_matrix);
      }
      device_->DrawContent();
    }

    DrawUI();
  }

 private:
  Camera camera_;
  SharedGPUDevice device_ = GPUDevice::Create();
  SharedShader lighted_shader_;
  SharedShader lighting_shader_;

  // ui states
  glm::vec4 entity_color_ = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
  glm::vec4 light_color_ = glm::vec4(1.0f);
  glm::vec3 light_position_ = glm::vec3(1, 1, -1);

  float ambient_ = 0.2f;
  float diffuse_ = 1.0f;
  float specular_ = 0.5f;
  float shininess_ = 32.0f;

  MyApp() { delegate_ = this; }

  void DrawUI() {
    ImGui::Begin("Properties");
    ImGui::ColorEdit4("light_color", glm::value_ptr(light_color_));
    ImGui::ColorEdit4("entity_color", glm::value_ptr(entity_color_));
    ImGui::SliderFloat3("light_position", glm::value_ptr(light_position_), -2,
                        2, "%.1f");
    ImGui::SliderFloat("ambient", &ambient_, 0.0f, 1.0f);
    ImGui::SliderFloat("diffuse", &diffuse_, 0.0f, 1.0f);
    ImGui::SliderFloat("specular", &specular_, 0.0f, 1.0f);
    ImGui::SliderFloat("shininess", &shininess_, 2.0f, 256.0f);
    ImGui::End();
  }
};

int main() {
  return MyApp::GetInstance()->Run();
}
