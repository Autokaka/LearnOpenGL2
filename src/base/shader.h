#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "base/scoped_gl_object.h"
#include "macros.h"

enum class ShaderType {
  kVertex = GL_VERTEX_SHADER,
  kFragment = GL_FRAGMENT_SHADER,
};

class Shader final {
 public:
  using SharedShader = std::shared_ptr<Shader>;

  static SharedShader CreateFromSource(
      const std::string& vertex_shader_source,
      const std::string& fragment_shader_source);
  static SharedShader CreateFromFile(const std::string& vertex_shader_path,
                                     const std::string& fragment_shader_path);
  ~Shader();

  void Use() const;

  void SetBool(const std::string& name, bool value) const;
  void SetInt(const std::string& name, int value) const;
  void SetFloat(const std::string& name, float value) const;

  void SetVec2(const std::string& name, glm::vec2 value) const;
  void SetVec3(const std::string& name, glm::vec3 value) const;
  void SetVec4(const std::string& name, glm::vec4 value) const;

  void SetMat2(const std::string& name, glm::mat2 value) const;
  void SetMat3(const std::string& name, glm::mat3 value) const;
  void SetMat4(const std::string& name, glm::mat4 value) const;

 private:
  GLuint id_;

  MAKE_SHARED_CONSTRUCTOR(Shader, Create);
  explicit Shader(uint32_t id);

  static SharedGLObject CompileShaderFromSource(const char* source,
                                                const ShaderType& shader_type);

  DISALLOW_COPY_ASSIGN_AND_MOVE(Shader);
};
using SharedShader = Shader::SharedShader;
