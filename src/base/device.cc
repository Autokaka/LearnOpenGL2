/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/20.
 */

#include "device.h"

GPUDevice::GPUDevice()
    : vbo_(nullptr), gl_vbo_(nullptr), current_shader_(nullptr) {}

void GPUDevice::UseVertexBuffer(const SharedVertexBuffer& vbo) {
  if (vbo_ == vbo) {
    return;
  }

  vbo_ = vbo;
  if (vbo_) {
    gl_vbo_ = vbo_->CreateGLObject();
  }
}

void GPUDevice::UseProgram(const SharedShader& shader) {
  if (current_shader_ == shader) {
    return;
  }

  current_shader_ = shader;
  if (current_shader_) {
    current_shader_->Use();
  }
}

void GPUDevice::DrawContents() {
  if (!vbo_ || !gl_vbo_ || !current_shader_) {
    return;
  }

  UseProgram(current_shader_);

  // If ebo exists, call glDrawElements
  if (auto gl_ebo_id = gl_vbo_->GetDrawSequence()) {
    glBindVertexArray(gl_ebo_id.value());
    glDrawElements(GL_TRIANGLES, vbo_->GetDrawSequence().size(),
                   GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
    glBindVertexArray(0);
    return;
  }

  // If vbo exists, call glDrawArrays
  if (auto gl_vbo_id = gl_vbo_->GetVertexInfo()) {
    glBindVertexArray(gl_vbo_id.value());
    glDrawArrays(GL_TRIANGLES, 0, vbo_->GetVertexCount());
    glBindVertexArray(0);
    return;
  }
}
