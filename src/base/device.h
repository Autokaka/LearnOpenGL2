/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/19.
 */

#pragma once

#include <unordered_map>
#include <vector>

#include "buffer.h"
#include "shader.h"
#include "texture.h"

class GPUDevice final {
 public:
  MAKE_SHARED_CONSTRUCTOR(GPUDevice, Create);

  void UseVertexBuffer(const SharedVertexBuffer& vbo);
  void UseProgram(const SharedShader& shader);
  void DrawContent();

 private:
  std::unordered_map<SharedVertexBuffer, SharedGLVertexBuffer> vbos_;
  SharedVertexBuffer current_vbo_;
  SharedGLVertexBuffer current_gl_vbo_;
  SharedShader current_shader_;

  GPUDevice();

  DISALLOW_COPY_ASSIGN_AND_MOVE(GPUDevice);
};
using SharedGPUDevice = std::shared_ptr<GPUDevice>;
