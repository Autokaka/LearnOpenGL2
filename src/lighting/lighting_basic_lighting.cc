// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
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

// NOTE(Autokaka): Output data should all based on world space here.
out vec3 v_position;
out vec3 v_normal;

void main() {
  gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
  v_position = vec3(u_model * vec4(a_position, 1.0));
  v_normal = mat3(u_normal_matrix) * a_normal;
}
)__SHADER__";
    const char* lighted_fsh = R"__SHADER__(
#version 330 core

uniform vec4 u_entity_color;
uniform vec4 u_light_color;
uniform vec3 u_light_position;

in vec3 v_position;
in vec3 v_normal;

out vec4 FragColor;

vec4 GetAmbientLightColor(float ambient) {
  return ambient * u_light_color;
}

vec4 GetDiffuseLightColor(vec3 frag_position, vec3 light_position, vec3 frag_normal) {
  vec3 light_direction = light_position - frag_position;
  float diffuse = max(dot(normalize(light_direction), normalize(frag_normal)), 0.0); 
  return diffuse * u_light_color;
}

void main() {
  vec4 ambient_color = GetAmbientLightColor(0.2);
  vec4 diffuse_color = GetDiffuseLightColor(v_position, u_light_position, v_normal);
  vec4 final_light_color = ambient_color + diffuse_color;
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

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    camera_.SetPosition(glm::vec3(2, 2, 3));
    camera_.LookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  }

  void DrawContent() override {
    // do actual rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec3 light_position = glm::vec3(1, 1, -1);
    glm::mat4 view = glm::inverse(camera_.GetTransformation());
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.f);

    {  // draw lighted entity
      device_->UseProgram(lighted_shader_);
      {
        lighted_shader_->SetVec4("u_entity_color",
                                 glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
        lighted_shader_->SetVec4("u_light_color",
                                 glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        lighted_shader_->SetVec3("u_light_position", light_position);
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
      {
        lighting_shader_->SetVec4("u_light_color",
                                  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
      }
      {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), light_position);
        model = glm::scale(model, glm::vec3(0.5f));
        glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
        lighting_shader_->SetMat4("u_model", model);
        lighting_shader_->SetMat4("u_view", view);
        lighting_shader_->SetMat4("u_projection", projection);
        lighting_shader_->SetMat4("u_normal_matrix", normal_matrix);
      }
      device_->DrawContent();
    }
  }

 private:
  Camera camera_;
  SharedGPUDevice device_ = GPUDevice::Create();
  SharedShader lighted_shader_;
  SharedShader lighting_shader_;

  MyApp() { delegate_ = this; }
};

int main() {
  return MyApp::GetInstance()->Run();
}
