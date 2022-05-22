// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "base/buffer.h"
#include "base/camera.h"
#include "base/device.h"
#include "base/shader.h"
#include "glm/fwd.hpp"

namespace {

Camera camera;
SharedGPUDevice device = GPUDevice::Create();

void OnWindowSizeChangedCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

#pragma mark - Position
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    camera.GoForward(0.1f);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    camera.GoBackward(0.1f);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    camera.GoLeft(0.1f);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    camera.GoRight(0.1f);
  }

#pragma mark - Rotation
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.PitchUp(1);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.PitchDown(1);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.YawLeft(1);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.YawRight(1);
  }
}

}  // namespace

int main() {
  // create an opengl window via glfw.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window =
      glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // initialize glad
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // initialize viewport
  glfwSetFramebufferSizeCallback(window, OnWindowSizeChangedCallback);
  int viewport_width, viewport_height;
  glfwGetFramebufferSize(window, &viewport_width, &viewport_height);
  glViewport(0, 0, viewport_width, viewport_height);

  // create VBO
  // clang-format off
  SharedVertexBuffer vbo = VertexBuffer::Create(std::vector<float>({
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  

    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 

     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f, 

    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f, -0.5f, 

    -0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f,  
  }), std::vector<Attribute>({
    { "a_position", AttributeType::kVec3 },
  }));
  // clang-format on
  device->UseVertexBuffer(vbo);

  // create shader program
  const char* common_vsh = R"__SHADER__(
#version 330 core
layout (location = 0) in vec3 a_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
  gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
)__SHADER__";
  const char* lighted_fsh = R"__SHADER__(
#version 330 core

uniform vec4 u_entity_color;
uniform vec4 u_light_color;

out vec4 FragColor;

void main() {
  FragColor = u_entity_color * u_light_color;
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
  auto lighted_shader_program =
      Shader::CreateFromSource(common_vsh, lighted_fsh);
  auto lighting_shader_program =
      Shader::CreateFromSource(common_vsh, lighting_fsh);
  if (!lighted_shader_program || !lighting_shader_program) {
    return -1;
  }

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  camera.SetPosition(glm::vec3(2, 2, 3));
  camera.LookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

  // start rendering loop
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    // do actual rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 view = glm::inverse(camera.GetTransformMatrix());
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.f);

    {  // draw lighted entity
      device->UseProgram(lighted_shader_program);
      lighted_shader_program->SetVec4("u_entity_color",
                                      glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
      lighted_shader_program->SetVec4("u_light_color",
                                      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
      lighted_shader_program->SetMat4(
          "u_model", glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
      lighted_shader_program->SetMat4("u_view", view);
      lighted_shader_program->SetMat4("u_projection", projection);
      device->DrawContent();
    }

    {  // draw lighting entity
      device->UseProgram(lighting_shader_program);
      lighted_shader_program->SetVec4("u_light_color",
                                      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
      glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, -1));
      model = glm::scale(model, glm::vec3(0.1f));
      lighted_shader_program->SetMat4("u_model", model);
      lighted_shader_program->SetMat4("u_view", view);
      lighted_shader_program->SetMat4("u_projection", projection);
      device->DrawContent();
    }

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
