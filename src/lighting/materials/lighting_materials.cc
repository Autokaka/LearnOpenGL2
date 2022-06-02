// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <imgui/imgui.h>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

#include "base/app.h"
#include "base/buffer.h"
#include "base/camera.h"
#include "base/render_pass.h"
#include "base/shader.h"
#include "lighting_materials_shaders.h"

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

  void WindowSizeDidChange(int width, int height) override { glViewport(0, 0, width, height); }

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
    SharedVertexBuffer vbo = VertexBuffer::Create(cube_vertices, std::vector<Attribute>({
      { "a_position", AttributeType::kVec3 },
      { "a_normal", AttributeType::kVec3 },
    }));
    // clang-format on
    render_pass_->UseVertexBuffer(vbo);

    // create shader program
    lighted_shader_ = Shader::CreateFromSource(common_vsh.data(), lighted_fsh.data());
    lighting_shader_ = Shader::CreateFromSource(common_vsh.data(), lighting_fsh.data());
    if (!lighted_shader_ || !lighting_shader_) {
      Exit(-1);
    }

    camera_.SetPosition(glm::vec3(-1, 0, 3));
    camera_.LookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
  }

  void DrawContent() override {
    // do actual rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 view = camera_.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.f);

    {  // draw lighted entity
      render_pass_->UseProgram(lighted_shader_);
      {
        lighted_shader_->SetVec3("u_view_position", camera_.GetPosition());

        lighted_shader_->SetVec3("u_light.position", light_.position);
        lighted_shader_->SetVec4("u_light.ambient", light_.ambient);
        lighted_shader_->SetVec4("u_light.diffuse", light_.diffuse);
        lighted_shader_->SetVec4("u_light.specular", light_.specular);

        lighted_shader_->SetVec4("u_material.ambient", material_.ambient);
        lighted_shader_->SetVec4("u_material.diffuse", material_.diffuse);
        lighted_shader_->SetVec4("u_material.specular", material_.specular);
        lighted_shader_->SetFloat("u_material.shininess", material_.shininess);
      }
      {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
        lighted_shader_->SetMat4("u_model", model);
        lighted_shader_->SetMat4("u_view", view);
        lighted_shader_->SetMat4("u_projection", projection);
        lighted_shader_->SetMat4("u_normal_matrix", normal_matrix);
      }
      render_pass_->DrawContent();
    }

    {  // draw lighting entity
      render_pass_->UseProgram(lighting_shader_);
      { lighting_shader_->SetVec4("u_light_color", light_.specular); }
      {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), light_.position);
        model = glm::scale(model, glm::vec3(0.2f));
        glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
        lighting_shader_->SetMat4("u_model", model);
        lighting_shader_->SetMat4("u_view", view);
        lighting_shader_->SetMat4("u_projection", projection);
        lighting_shader_->SetMat4("u_normal_matrix", normal_matrix);
      }
      render_pass_->DrawContent();
    }

    DrawUI();
  }

 private:
  Camera camera_;
  SharedRenderPass render_pass_ = RenderPass::Create();
  SharedShader lighted_shader_;
  SharedShader lighting_shader_;

  // ui states
  class PhongLight {
   public:
    glm::vec3 position{1.2f, 1.0f, 2.0f};
    glm::vec4 ambient{0.2f, 0.2f, 0.2f, 1.0f};
    glm::vec4 diffuse{0.5f, 0.5f, 0.5f, 1.0f};
    glm::vec4 specular{1.0f};
  } light_;

  class PhongMaterial {
   public:
    glm::vec4 ambient{1.0f, 0.5f, 0.31f, 1.0f};
    glm::vec4 diffuse{1.0f, 0.5f, 0.31f, 1.0f};
    glm::vec4 specular{0.5f, 0.5f, 0.5f, 1.0f};
    float shininess = 32.0f;
  } material_;

  MyApp() { delegate_ = this; }

  void DrawUI() {
    ImGui::Begin("Properties");

    ImGui::SliderFloat3("light.position", glm::value_ptr(light_.position), -2, 2, "%.1f");
    ImGui::ColorEdit4("light.ambient", glm::value_ptr(light_.ambient));
    ImGui::ColorEdit4("light.diffuse", glm::value_ptr(light_.diffuse));
    ImGui::ColorEdit4("light.specular", glm::value_ptr(light_.specular));

    ImGui::ColorEdit4("material.ambient", glm::value_ptr(material_.ambient));
    ImGui::ColorEdit4("material.diffuse", glm::value_ptr(material_.diffuse));
    ImGui::ColorEdit4("material.specular", glm::value_ptr(material_.specular));
    ImGui::SliderFloat("material.shininess", &material_.shininess, 2.0f, 256.0f);

    ImGui::End();
  }
};

int main() {
  return MyApp::GetInstance()->Run();
}
