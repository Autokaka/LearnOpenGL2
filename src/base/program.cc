/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/29.
 */

#include "program.h"
#include "type_guard.h"

Program::Program() : uniform_providers_({}) {}

void Program::SetVertexShader(const std::string_view& vertex_shader) {
  if (vertex_shader_ != vertex_shader) {
    vertex_shader_ = vertex_shader;
    gl_object_ = nullptr;
  }
}

void Program::SetFragmentShader(const std::string_view& fragment_shader) {
  if (fragment_shader_ != fragment_shader) {
    fragment_shader_ = fragment_shader;
    gl_object_ = nullptr;
  }
}

void Program::SetUniformProviders(const UniformProviderMap& providers) {
  uniform_providers_ = providers;
}

void Program::SetUniformProvider(std::string_view name, UniformProvider provider) {
  uniform_providers_[name] = provider;
}

SharedShader Program::MakeGLObject() {
  if (gl_object_) {
    SubmitCommands();
    return gl_object_;
  }

  gl_object_ = Shader::CreateFromSource(vertex_shader_.data(), fragment_shader_.data());
  return gl_object_ ? MakeGLObject() : nullptr;
}

void Program::SubmitCommands() {
  if (gl_object_) {
    gl_object_->Use();
    for (const auto& [name, uniform_provider] : uniform_providers_) {
      const auto uniform_value = uniform_provider();
      if (IsBool(uniform_value)) {
        gl_object_->SetBool(name.data(), std::any_cast<bool>(uniform_value));
      } else if (IsInt(uniform_value)) {
        gl_object_->SetInt(name.data(), std::any_cast<int>(uniform_value));
      } else if (IsFloat(uniform_value)) {
        gl_object_->SetFloat(name.data(), std::any_cast<float>(uniform_value));
      } else if (IsVec2(uniform_value)) {
        gl_object_->SetVec2(name.data(), std::any_cast<glm::vec2>(uniform_value));
      } else if (IsVec3(uniform_value)) {
        gl_object_->SetVec3(name.data(), std::any_cast<glm::vec3>(uniform_value));
      } else if (IsVec4(uniform_value)) {
        gl_object_->SetVec4(name.data(), std::any_cast<glm::vec4>(uniform_value));
      } else if (IsMat2(uniform_value)) {
        gl_object_->SetMat2(name.data(), std::any_cast<glm::mat2>(uniform_value));
      } else if (IsMat3(uniform_value)) {
        gl_object_->SetMat3(name.data(), std::any_cast<glm::mat3>(uniform_value));
      } else if (IsMat4(uniform_value)) {
        gl_object_->SetMat4(name.data(), std::any_cast<glm::mat4>(uniform_value));
      } else if (IsSampler2D(uniform_value)) {
        gl_object_->SetSampler2D(name.data(), std::any_cast<SharedTexture>(uniform_value));
      }
    }
  }
}
