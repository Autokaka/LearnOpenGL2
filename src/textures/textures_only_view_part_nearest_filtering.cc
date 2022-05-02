// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include "glm/fwd.hpp"
// clang-format on

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <algorithm>
#include <iostream>

#include "base/shader.h"

namespace {

glm::vec2 view_offset = {0, 0};

void OnWindowSizeChangedCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window, const Shader& shader_program) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    view_offset.x = std::max(0.0f, view_offset.x - 0.01f);
    shader_program.SetVec2("uViewOffset", view_offset);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    view_offset.x = std::min(0.5f, view_offset.x + 0.01f);
    shader_program.SetVec2("uViewOffset", view_offset);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    view_offset.y = std::min(0.5f, view_offset.y + 0.01f);
    shader_program.SetVec2("uViewOffset", view_offset);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    view_offset.y = std::max(0.0f, view_offset.y - 0.01f);
    shader_program.SetVec2("uViewOffset", view_offset);
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

  // create textures
  uint32_t textures[2] = {0};
  glGenTextures(2, textures);
  stbi_set_flip_vertically_on_load(true);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  int width, height, nr_channels;
  u_char* data = stbi_load("container.jpg", &width, &height, &nr_channels, 0);
  if (data) {
    glTexImage2D(/** define texture format in gl */ GL_TEXTURE_2D, 0, GL_RGB,
                 width, height, 0,
                 /** define raw container.jpg */ GL_RGB, GL_UNSIGNED_BYTE,
                 data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  data = stbi_load("awesomeface.png", &width, &height, &nr_channels, 0);
  if (data) {
    glTexImage2D(/** define texture format in gl */ GL_TEXTURE_2D, 0, GL_RGB,
                 width, height, 0,
                 /** define raw awesomeface.png */ GL_RGBA, GL_UNSIGNED_BYTE,
                 data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  // create VAO
  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);  // [VAO STATE] BEGIN

  // create VBO
  // clang-format off
  float vertices[] = {
    /** layout location(0) vec3 aPos */ -0.5f, -0.5f, 0.0f, /** layout location(1) vec3 aTexCoord */ 0.0f, 0.0f,
    /** layout location(0) vec3 aPos */ -0.5f, 0.5f, 0.0f, /** layout location(1) vec3 aTexCoord */ 0.0f, 0.5f,
    /** layout location(0) vec3 aPos */ 0.5f, 0.5f, 0.0f, /** layout location(1) vec3 aTexCoord */ 0.5f, 0.5f,
    /** layout location(0) vec3 aPos */ 0.5f, -0.5f, 0.0f, /** layout location(1) vec3 aTexCoord */ 0.5f, 0.0f,
  };
  // clang-format on
  uint VBO;
  glGenBuffers(1, &VBO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // [VBO STATE] BEGIN
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // append vertex attribute
    // read vec3 aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // read vec2 aTexCoord
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // [VBO STATE] END
  }

  // create EBO
  // clang-format off
  uint32_t indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  // clang-format on
  uint32_t EBO;
  glGenBuffers(1, &EBO);
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  // [EBO STATE] BEGIN
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);
  }

  // unbind VAO for later use
  glBindVertexArray(0);                      // [VAO STATE] END
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // [EBO STATE] END

  // create shader program
  const char* vertexShaderSource = R"__SHADER__(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main() {
  gl_Position = vec4(aPos, 1.0);
  vTexCoord = aTexCoord;
}
)__SHADER__";
  const char* fragmentShaderSource = R"__SHADER__(
#version 330 core
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform vec2 uViewOffset;

void main() {
  FragColor = mix(texture(uTexture0, vTexCoord), texture(uTexture1, vTexCoord + uViewOffset), 0.2);
}
)__SHADER__";
  auto shader_program =
      Shader::CreateFromSource(vertexShaderSource, fragmentShaderSource);
  if (!shader_program) {
    return -1;
  }

  // start rendering loop
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window, *shader_program.get());

    // do actual rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader_program->Use();

    // set uniforms
    shader_program->SetInt("uTexture0", 0);
    shader_program->SetInt("uTexture1", 1);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                   reinterpret_cast<void*>(0));
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}
