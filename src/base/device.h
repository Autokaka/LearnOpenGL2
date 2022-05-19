/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/19.
 */

#pragma once

#include "buffer.h"
#include "shader.h"

class GPUDevice final {
 public:
  MAKE_SHARED_CONSTRUCTOR(GPUDevice, Create);

  void UseVertexBuffer(const SharedVertexBuffer& vbo);
  void UseProgram(const SharedShader& shader);
  void DrawContents();

 private:
  SharedVertexBuffer vbo_;
  SharedGLVertexBuffer gl_vbo_;
  SharedShader current_shader_;

  GPUDevice();

  DISALLOW_COPY_ASSIGN_AND_MOVE(GPUDevice);
};
using SharedGPUDevice = std::shared_ptr<GPUDevice>;
