/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/17.
 */

#include <glad/glad.h>

#include "buffer.h"

#pragma mark - Attribute

Attribute::Attribute(const std::string& name, AttributeType type, int size)
    : name_(name), type_(type) {
  if (type == AttributeType::kCustom) {
    size_ = size;
    return;
  }
  size_ = static_cast<int>(type);
}

#pragma mark - GLVertexBuffer

GLVertexBuffer::GLVertexBuffer(const SharedGLObject& vertex_info,
                               const SharedGLObject& draw_sequence)
    : vertex_info_(vertex_info), draw_sequence_(draw_sequence) {}

std::optional<uint32_t> GLVertexBuffer::GetVertexInfo() const {
  if (vertex_info_) {
    return vertex_info_->GetId();
  }
  return std::nullopt;
}
std::optional<uint32_t> GLVertexBuffer::GetDrawSequence() const {
  if (draw_sequence_) {
    return draw_sequence_->GetId();
  }
  return std::nullopt;
}

#pragma mark - VertexBuffer

VertexBuffer::VertexBuffer(const VertexInfo& vertex_info,
                           const AttributeList& attribute_list,
                           const DrawSequence& draw_sequence) {
  SetVertexInfo(vertex_info);
  SetAttributeList(attribute_list);
  SetDrawSequence(draw_sequence);
}

void VertexBuffer::SetVertexInfo(const VertexInfo& vertex_info) {
  vertex_info_ = vertex_info;
}

void VertexBuffer::SetAttributeList(const AttributeList& attribute_list) {
  attribute_list_ = attribute_list;

  // Update one_vertex_size_
  if (attribute_list_.empty()) {
    one_vertex_size_ = 1;
  }

  one_vertex_size_ = 0;
  for (auto&& attribute : attribute_list_) {
    one_vertex_size_ += attribute.GetSize();
  }
}

void VertexBuffer::SetDrawSequence(const DrawSequence& draw_sequence) {
  draw_sequence_ = draw_sequence;
}

SharedGLVertexBuffer VertexBuffer::CreateGLObject() const {
  if (IsEmpty()) {
    return nullptr;
  }

  // vao
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // vbo
  const SharedGLObject vbo = ScopedGLObject::Create(
      []() -> GLuint {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        return vbo;
      },
      [](GLuint id) { glDeleteBuffers(1, &id); });
  const int vbo_size = vertex_info_.size() * sizeof(float);
  glBindBuffer(GL_ARRAY_BUFFER, vbo->GetId());
  glBufferData(GL_ARRAY_BUFFER, vbo_size, vertex_info_.data(), GL_STATIC_DRAW);
  int channel = 0, offset = 0;
  for (auto&& attribute : attribute_list_) {
    glVertexAttribPointer(channel, attribute.GetSize(), GL_FLOAT, GL_FALSE,
                          one_vertex_size_ * sizeof(float),
                          reinterpret_cast<void*>(offset * sizeof(float)));
    glEnableVertexAttribArray(channel);
    channel += 1;
    offset += attribute.GetSize();
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // ebo
  SharedGLObject ebo = nullptr;
  if (!draw_sequence_.empty()) {
    ebo = ScopedGLObject::Create(
        []() -> GLuint {
          GLuint ebo;
          glGenBuffers(1, &ebo);
          return ebo;
        },
        [](GLuint id) { glDeleteBuffers(1, &id); });
    const int ebo_size = draw_sequence_.size() * sizeof(uint32_t);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->GetId());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_size, draw_sequence_.data(),
                 GL_STATIC_DRAW);
  }

  glBindVertexArray(0);
  if (ebo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  return GLVertexBuffer::Create(vbo, ebo);
}
