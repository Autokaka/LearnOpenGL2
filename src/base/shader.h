/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/04/28.
 */

#pragma once

#include <glm/glm.hpp>
#include <mutex>

#include "macros.h"
#include "scoped_gl_object.h"
#include "texture.h"

enum class ShaderType {
  kVertex = GL_VERTEX_SHADER,
  kFragment = GL_FRAGMENT_SHADER,
};

class Shader final : public std::enable_shared_from_this<Shader> {
 public:
  using SharedShader = std::shared_ptr<Shader>;

  static SharedShader CreateFromSource(const std::string_view& vertex_shader_source,
                                       const std::string_view& fragment_shader_source);
  static SharedShader CreateFromFile(const std::string_view& vertex_shader_path,
                                     const std::string_view& fragment_shader_path);
  static int GetTextureUnitLimitPerFragmentShader();
  static int GetTextureUnitLimitPerShader();

  ~Shader();

  uint32_t GetId() const { return id_; }

  void Use();

  void SetBool(const std::string_view& name, bool value) const;
  void SetInt(const std::string_view& name, int value) const;
  void SetFloat(const std::string_view& name, float value) const;

  void SetVec2(const std::string_view& name, const glm::vec2& value) const;
  void SetVec3(const std::string_view& name, const glm::vec3& value) const;
  void SetVec4(const std::string_view& name, const glm::vec4& value) const;

  void SetMat2(const std::string_view& name, const glm::mat2& value) const;
  void SetMat3(const std::string_view& name, const glm::mat3& value) const;
  void SetMat4(const std::string_view& name, const glm::mat4& value) const;

  void SetSampler2D(const std::string_view& name, const SharedTexture& texture);

 private:
  static int texture_unit_limit_per_fragmet_shader_;
  static int texture_unit_limit_per_shader_;
  static std::once_flag is_texture_unit_limits_queried_;

  GLuint id_;
  GLuint unit_;

  MAKE_SHARED_CONSTRUCTOR(Shader, Create);
  explicit Shader(uint32_t id);

  static SharedGLObject CompileShaderFromSource(const char* source, const ShaderType& shader_type);

  DISALLOW_COPY_ASSIGN_AND_MOVE(Shader);
};
using SharedShader = Shader::SharedShader;
