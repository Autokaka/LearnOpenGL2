/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/29.
 */

#include <any>
#include <string_view>
#include <unordered_map>

#include "gpu_access.h"
#include "shader.h"

class Program : public GPUAccess<SharedShader>, std::enable_shared_from_this<Program> {
 public:
  using SharedProgram = std::shared_ptr<Program>;
  using UniformProvider = std::function<std::any()>;
  using UniformProviderMap = std::unordered_map<std::string_view, UniformProvider>;

  MAKE_SHARED_CONSTRUCTOR(Program, Create);

  std::string_view GetVertexShader() const { return vertex_shader_; }
  void SetVertexShader(const std::string_view& vertex_shader);
  std::string_view GetFragmentShader() const { return fragment_shader_; }
  void SetFragmentShader(const std::string_view& fragment_shader);

  void SetUniformProviders(const UniformProviderMap& providers);
  void SetUniformProvider(std::string_view name, UniformProvider provider);

  SharedShader MakeGLObject() override;

 protected:
  Program();

  void SubmitCommands() override;

 private:
  std::string_view vertex_shader_;
  std::string_view fragment_shader_;
  UniformProviderMap uniform_providers_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Program);
};
using SharedProgram = Program::SharedProgram;
