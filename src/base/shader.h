#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "macros.h"
#include "scoped_gl_object.h"

class Shader final : public ScopedGLObject {
 public:
  static std::unique_ptr<Shader> CreateFromSource(
      const std::string& vertex_shader_source,
      const std::string& fragment_shader_source);
  static std::unique_ptr<Shader> CreateFromFile(
      const std::string& vertex_shader_path,
      const std::string& fragment_shader_path);
  explicit Shader(uint32_t id) : ScopedGLObject(id, glDeleteProgram) {}

  void Use() const;

  void SetBool(const std::string& name, bool value) const;
  void SetInt(const std::string& name, int value) const;
  void SetFloat(const std::string& name, float value) const;
  void SetVec2(const std::string& name, glm::vec2 value) const;
  void SetVec3(const std::string& name, glm::vec3 value) const;
  void SetVec4(const std::string& name, glm::vec4 value) const;

 private:
  static SharedGLObject CompileShaderFromSource(const std::string& source,
                                                int shader_type,
                                                const char* log = nullptr);
};
