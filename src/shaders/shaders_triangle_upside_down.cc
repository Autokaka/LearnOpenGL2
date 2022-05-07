// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <complex>
#include <iostream>

#include "base/shader.h"

void OnWindowSizeChangedCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

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

  // create VAO
  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);  // [VAO STATE] BEGIN

  // create VBO
  // clang-format off
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
  };
  // clang-format on
  uint VBO;
  glGenBuffers(1, &VBO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // [VBO STATE] BEGIN
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // append vertex attribute
    // read vec3 aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // [VBO STATE] END
  }

  // unbind VAO for later use
  glBindVertexArray(0);  // [VAO STATE] END

  // create shader program
  const char* vertexShaderSource = R"__SHADER__(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
}
)__SHADER__";
  const char* fragmentShaderSource = R"__SHADER__(
#version 330 core
uniform vec4 changingColor;

out vec4 FragColor;

void main() {
  FragColor = changingColor;
}
)__SHADER__";
  auto shader_program =
      Shader::CreateFromSource(vertexShaderSource, fragmentShaderSource);
  if (!shader_program) {
    return -1;
  }

  // start rendering loop
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    // do actual rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader_program->Use();

    // set uniform (changingColor) value
    float timeInSeconds = glfwGetTime();
    float greenValue = (std::sin(timeInSeconds) / 2.0f) + 0.5f;
    shader_program->SetVec4("changingColor",
                            glm::vec4(0.0f, greenValue, 0.0f, 1.0f));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}
