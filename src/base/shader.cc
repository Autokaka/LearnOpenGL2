#include <fstream>
#include <iostream>
#include <sstream>

#include "shader.h"

std::unique_ptr<Shader> Shader::CreateFromSource(
    const std::string& vertex_shader_source,
    const std::string& fragment_shader_source) {
  // create shaders
  char log[512];
  auto vertex_shader =
      CompileShaderFromSource(vertex_shader_source, GL_VERTEX_SHADER, log);
  if (!vertex_shader) {
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << log
              << std::endl;
    return nullptr;
  }
  auto fragment_shader =
      CompileShaderFromSource(fragment_shader_source, GL_FRAGMENT_SHADER, log);
  if (!fragment_shader) {
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << log
              << std::endl;
    return nullptr;
  }

  // create program
  uint program_id;
  int success;
  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader->GetId());
  glAttachShader(program_id, fragment_shader->GetId());
  glLinkProgram(program_id);
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_id, 512, nullptr, log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << log << std::endl;
    return nullptr;
  }
  return std::make_unique<Shader>(program_id);
}

std::unique_ptr<Shader> Shader::CreateFromFile(
    const std::string& vertex_shader_path,
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
    std::cout << "ERROR::SHADER::FAILED_TO_READ_FILE" << std::endl;
  }

  return Shader::CreateFromSource(vertex_shader_code, fragment_shader_code);
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

SharedGLObject Shader::CompileShaderFromSource(const std::string& source,
                                               int shader_type,
                                               const char* log) {
  const char* shader_source_code = source.c_str();

  uint shader_id;
  int success;
  char info_log[512];

  shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &shader_source_code, nullptr);
  glCompileShader(shader_id);
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
    return nullptr;
  }

  return ScopedGLObject::MakeShared(shader_id, glDeleteShader);
}
