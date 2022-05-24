/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/20.
 */

#include "device.h"

GPUDevice::GPUDevice()
    : vbos_({}),
      current_vbo_(nullptr),
      current_gl_vbo_(nullptr),
      current_shader_(nullptr) {}

void GPUDevice::UseVertexBuffer(const SharedVertexBuffer& vbo) {
  if (!vbo) {
    current_vbo_ = nullptr;
    current_gl_vbo_ = nullptr;
    return;
  }

  current_vbo_ = vbo;
  if (auto gl_vbo = vbos_[vbo]) {
    current_gl_vbo_ = gl_vbo;
  } else {
    current_gl_vbo_ = current_vbo_->CreateGLObject();
  };

  if (current_gl_vbo_) {
    current_vbo_->SubmitCommands(current_gl_vbo_);
  }
}

void GPUDevice::UseProgram(const SharedShader& shader) {
  current_shader_ = shader;

  if (current_shader_) {
    current_shader_->Use();
  }
}

void GPUDevice::DrawContent() {
  if (!current_gl_vbo_) {
    return;
  }

  if (auto gl_ebo_id = current_gl_vbo_->GetDrawSequence()) {
    // If ebo exists, call glDrawElements
    glBindVertexArray(current_gl_vbo_->GetId());
    glDrawElements(GL_TRIANGLES, current_vbo_->GetDrawSequence().size(),
                   GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
    glBindVertexArray(0);
    return;
  }

  if (auto gl_vbo_id = current_gl_vbo_->GetVertexInfo()) {
    // If vbo exists, call glDrawArrays
    glBindVertexArray(current_gl_vbo_->GetId());
    glDrawArrays(GL_TRIANGLES, 0, current_vbo_->GetVertexCount());
    glBindVertexArray(0);
    return;
  }
}
