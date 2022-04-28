#pragma once

#include <sys/types.h>
#include <memory>
#include <string>

#include "macros.h"

class Shader {
 public:
  static std::unique_ptr<Shader> CreateFromSource(
      const std::string& vertex_shader_source,
      const std::string& fragment_shader_source);
  static std::unique_ptr<Shader> CreateFromFile(
      const std::string& vertex_shader_path,
      const std::string& fragment_shader_path);
  explicit Shader() = default;
  ~Shader();

  uint GetId() const { return id_; }

  void Use() const;

  void SetBool(const std::string& name, bool value) const;
  void SetInt(const std::string& name, int value) const;
  void SetFloat(const std::string& name, float value) const;

 private:
  static int CompileShaderFromSource(const std::string& source,
                                     int shader_type,
                                     const char* log = nullptr);

  uint id_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Shader);
};
