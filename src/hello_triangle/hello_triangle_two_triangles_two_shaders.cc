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
  glfwSetFramebufferSizeCallback(window, OnWindowSizeChangedCallback);
  int viewport_width, viewport_height;
  glfwGetFramebufferSize(window, &viewport_width, &viewport_height);
  glViewport(0, 0, viewport_width, viewport_height);

  // create VAO & VBO
  uint VAOs[2], VBOs[2];
  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);

  //////////////////////////////////////////////////////////////////////////////////////////

  // bind VAO_0
  glBindVertexArray(VAOs[0]);  // [VAO_0 STATE] BEGIN

  // bind VBO_0
  // clang-format off
  float vertices0[] = {
    0.5f, 0.5f, 0.0f,
    0.1f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
  };
  // clang-format on
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);  // [VBO_0 STATE] BEGIN
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);

    // append vertex attribute
    // read vec3 aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // [VBO_0 STATE] END
  }

  glBindVertexArray(0);  // [VAO_0 STATE] END

  //////////////////////////////////////////////////////////////////////////////////////////

  // bind VAO_1
  glBindVertexArray(VAOs[1]);  // [VAO_1 STATE] BEGIN

  // bind VBO_1
  // clang-format off
  float vertices1[] = {
     -0.5f, 0.5f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -0.1f, 0.0f, 0.0f,
  };
  // clang-format on
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);  // [VBO_1 STATE] BEGIN
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    // append vertex attribute
    // read vec3 aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // [VBO_1 STATE] END
  }

  glBindVertexArray(0);  // [VAO_1 STATE] END

  //////////////////////////////////////////////////////////////////////////////////////////

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

  //////////////////////////////////////////////////////////////////////////////////////////

  // create orange fragment shader
  const char* orangeFragmentShaderSource = R"__SHADER__(
#version 330 core
out vec4 FragColor;

void main() {
  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)__SHADER__";
  uint orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(orangeFragmentShader, 1, &orangeFragmentShaderSource, nullptr);
  glCompileShader(orangeFragmentShader);
  glGetShaderiv(orangeFragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(orangeFragmentShader, 512, nullptr, infoLog);
    std::cout << "ERROR::ORANGE_SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // create orange shader program
  uint orangeShaderProgram = glCreateProgram();
  glAttachShader(orangeShaderProgram, vertexShader);
  glAttachShader(orangeShaderProgram, orangeFragmentShader);
  glLinkProgram(orangeShaderProgram);
  glGetShaderiv(orangeShaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(orangeShaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR::ORANGE_SHADER::PROGRAM::LINK_FAILED\n"
              << infoLog << std::endl;
  }

  //////////////////////////////////////////////////////////////////////////////////////////

  // create yellow fragment shader
  const char* yellowFragmentShaderSource = R"__SHADER__(
#version 330 core
out vec4 FragColor;

void main() {
  FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
)__SHADER__";
  uint yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, nullptr);
  glCompileShader(yellowFragmentShader);
  glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(yellowFragmentShader, 512, nullptr, infoLog);
    std::cout << "ERROR::YELLOW_SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // create orange shader program
  uint yellowShaderProgram = glCreateProgram();
  glAttachShader(yellowShaderProgram, vertexShader);
  glAttachShader(yellowShaderProgram, yellowFragmentShader);
  glLinkProgram(yellowShaderProgram);
  glGetShaderiv(yellowShaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(yellowShaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR::YELLOW_SHADER::PROGRAM::LINK_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(yellowFragmentShader);
  glDeleteShader(orangeFragmentShader);
  glDeleteShader(vertexShader);

  // start rendering loop
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    // do actual rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(orangeShaderProgram);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glUseProgram(yellowShaderProgram);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteProgram(yellowShaderProgram);
  glDeleteProgram(orangeShaderProgram);
  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glfwTerminate();
  return 0;
}
