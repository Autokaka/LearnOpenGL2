/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/20.
 */

#include "render_pass.h"

RenderPass::RenderPass() : current_vbo_(nullptr), current_program_(nullptr) {}

void RenderPass::UseVertexBuffer(const SharedVertexBuffer& vbo) {
  current_vbo_ = vbo;
}

void RenderPass::UseProgram(const SharedShader& shader) {
  current_shader_ = shader;
  if (current_shader_) {
    current_shader_->Use();
  }
}

void RenderPass::UseProgram(const SharedProgram& program) {
  current_program_ = program;
}

void RenderPass::DrawContent() {
  if (current_vbo_ && current_program_) {
    const auto gl_program = current_program_->MakeGLObject();
    if (gl_program) {
      glUseProgram(gl_program->GetId());
    }

    const auto gl_vao = current_vbo_->MakeGLObject();
    if (gl_vao) {
      if (const auto gl_ebo = gl_vao->GetDrawSequence()) {
        // If ebo exists, call glDrawElements
        glBindVertexArray(gl_vao->GetId());
        glDrawElements(GL_TRIANGLES, current_vbo_->GetDrawSequence().size(), GL_UNSIGNED_INT,
                       reinterpret_cast<void*>(0));
        glBindVertexArray(0);
        return;
      }

      if (const auto gl_vbo = gl_vao->GetVertexInfo()) {
        // If vbo exists, call glDrawArrays
        glBindVertexArray(gl_vao->GetId());
        glDrawArrays(GL_TRIANGLES, 0, current_vbo_->GetVertexCount());
        glBindVertexArray(0);
        return;
      }
    }

    if (gl_program) {
      glUseProgram(0);
    }
  }

  // TODO(Autokaka): Remove the codes below in next release version of
  // forge-engine.
  else if (current_vbo_ && current_shader_) {
    const auto gl_vao = current_vbo_->MakeGLObject();
    if (gl_vao) {
      if (const auto gl_ebo = gl_vao->GetDrawSequence()) {
        // If ebo exists, call glDrawElements
        glBindVertexArray(gl_vao->GetId());
        glDrawElements(GL_TRIANGLES, current_vbo_->GetDrawSequence().size(), GL_UNSIGNED_INT,
                       reinterpret_cast<void*>(0));
        glBindVertexArray(0);
        return;
      }

      if (const auto gl_vbo = gl_vao->GetVertexInfo()) {
        // If vbo exists, call glDrawArrays
        glBindVertexArray(gl_vao->GetId());
        glDrawArrays(GL_TRIANGLES, 0, current_vbo_->GetVertexCount());
        glBindVertexArray(0);
        return;
      }
    }
  }
}
