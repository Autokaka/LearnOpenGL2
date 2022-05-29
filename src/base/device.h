/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/19.
 */

#pragma once

#include <vector>

#include "buffer.h"
#include "program.h"
#include "shader.h"
#include "texture.h"

class GPUDevice final {
 public:
  MAKE_SHARED_CONSTRUCTOR(GPUDevice, Create);

  void UseVertexBuffer(const SharedVertexBuffer& vbo);
  [[deprecated(
      "Shader will not be used directly in next version of forge-engine, "
      "please use GPUDevice::UseProgram(const SharedProgram& "
      "program) instead.")]] void
  UseProgram(const SharedShader& shader);
  void UseProgram(const SharedProgram& program);
  void DrawContent();

 private:
  SharedVertexBuffer current_vbo_;
  SharedProgram current_program_;
  [[deprecated(
      "Remove this feature in next release version of "
      "forge-engine.")]] SharedShader current_shader_;

  GPUDevice();

  DISALLOW_COPY_ASSIGN_AND_MOVE(GPUDevice);
};
using SharedGPUDevice = std::shared_ptr<GPUDevice>;
