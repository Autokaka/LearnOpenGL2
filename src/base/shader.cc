#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

#include "shader.h"

SharedShader Shader::CreateFromSource(
    const std::string& vertex_shader_source,
    const std::string& fragment_shader_source) {
  // create shaders
  SharedGLObject vertex_shader, fragment_shader;

  try {
    vertex_shader = CompileShaderFromSource(vertex_shader_source.c_str(),
                                            ShaderType::kVertex);
  } catch (const std::string& e) {
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << e
              << std::endl;
    return nullptr;
  }

  try {
    fragment_shader = CompileShaderFromSource(fragment_shader_source.c_str(),
                                              ShaderType::kFragment);
  } catch (const std::string& e) {
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << e
              << std::endl;
    return nullptr;
  }

  // create program
  uint program_id;
  int success;
  char exception[512];
  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader->GetId());
  glAttachShader(program_id, fragment_shader->GetId());
  glLinkProgram(program_id);
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_id, 512, nullptr, exception);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << exception
              << std::endl;
    return nullptr;
  }

  return std::shared_ptr<Shader>(new Shader(program_id));
}

SharedShader Shader::CreateFromFile(const std::string& vertex_shader_path,
                                    const std::string& fragment_shader_path) {
  // load shaders from file
  std::ifstream vertex_shader_file;
  std::ifstream fragment_shader_file;
  vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragment_shader_file.exceptions(std::ifstream::failbit |
                                  std::ifstream::badbit);

  std::string vertex_shader_code;
  std::string fragment_shader_code;
  try {
    vertex_shader_file.open(vertex_shader_path);
    fragment_shader_file.open(fragment_shader_path);
    std::stringstream vertex_shader_stream, fragment_shader_stream;
    vertex_shader_stream << vertex_shader_file.rdbuf();
    fragment_shader_stream << fragment_shader_file.rdbuf();
    vertex_shader_file.close();
    fragment_shader_file.close();

    vertex_shader_code = vertex_shader_stream.str();
    fragment_shader_code = fragment_shader_stream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FAILED_TO_READ_FILE: " << e.what()
              << std::endl;
  }

  return Shader::CreateFromSource(vertex_shader_code, fragment_shader_code);
}

Shader::Shader(uint32_t id) : id_(id) {}

Shader::~Shader() {
  glDeleteProgram(id_);
}

void Shader::Use() const {
  glUseProgram(id_);
}

void Shader::SetBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, glm::vec2 value) const {
  glUniform2f(glGetUniformLocation(id_, name.c_str()), value[0], value[1]);
}

void Shader::SetVec3(const std::string& name, glm::vec3 value) const {
  glUniform3f(glGetUniformLocation(id_, name.c_str()), value[0], value[1],
              value[2]);
}

void Shader::SetVec4(const std::string& name, glm::vec4 value) const {
  glUniform4f(glGetUniformLocation(id_, name.c_str()), value[0], value[1],
              value[2], value[3]);
}

void Shader::SetMat2(const std::string& name, glm::mat2 value) const {
  glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void Shader::SetMat3(const std::string& name, glm::mat3 value) const {
  glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) const {
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

SharedGLObject Shader::CompileShaderFromSource(const char* source,
                                               const ShaderType& shader_type) {
  if (!source) {
    throw "Empty shader source code";
  }

  int success;
  char exception[512];

  const auto shader = ScopedGLObject::Create(
      [&shader_type]() -> GLuint {
        return glCreateShader(static_cast<GLenum>(shader_type));
      },
      glDeleteShader);
  glShaderSource(shader->GetId(), 1, &source, nullptr);
  glCompileShader(shader->GetId());
  glGetShaderiv(shader->GetId(), GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader->GetId(), 512, nullptr, exception);
    throw std::string(exception);
  }

  return shader;
}
