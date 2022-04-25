// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>

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
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, OnWindowSizeChangedCallback);

  // create VBO
  // clang-format off
  float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
  };
  // clang-format on
  uint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // create VAO
  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // append vertex attribute
  // read vec3 aPos
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  // append EBO
  // clang-format off
  uint indices[] = {
    0, 1, 3, 
    1, 2, 3
  };
  // clang-format on
  uint EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // unbind VAO for later use
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // create vertex shader
  const char* vertexShaderSource = R"__SHADER__(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)__SHADER__";
  uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // create fragment shader
  const char* fragmentShaderSource = R"__SHADER__(
#version 330 core
out vec4 FragColor;

void main() {
  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)__SHADER__";
  uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // create shader program
  uint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // start rendering loop
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    // do actual rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                   reinterpret_cast<void*>(0));
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}
