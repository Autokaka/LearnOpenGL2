/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/17.
 */

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

#pragma mark - VertexBuffer

VertexBuffer::VertexBuffer(const std::vector<float>& data,
                           const AttributeList& attribute_list)
    : Buffer(data) {
  glGenBuffers(1, &id_);
  SetAttributeList(attribute_list);
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &id_);
}

void VertexBuffer::SetAttributeList(const AttributeList& attribute_list) {
  attribute_list_ = attribute_list;
  UpdateOneVertexSize();
}

void VertexBuffer::Submit() const {
  if (data_.empty()) {
    return;
  }

  const int size_in_bytes = data_.size() * sizeof(float);
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferData(GL_ARRAY_BUFFER, size_in_bytes, data_.data(), GL_STATIC_DRAW);
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
}

void VertexBuffer::UpdateOneVertexSize() {
  if (attribute_list_.empty()) {
    one_vertex_size_ = 1;
    return;
  }

  one_vertex_size_ = 0;
  for (auto&& attribute : attribute_list_) {
    one_vertex_size_ += attribute.GetSize();
  }
}

#pragma mark - IndexBuffer

IndexBuffer::IndexBuffer(const std::vector<uint32_t>& data) : Buffer(data) {
  glGenBuffers(1, &id_);
}

IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &id_);
}

void IndexBuffer::Submit() const {
  if (data_.empty()) {
    return;
  }

  const int size_in_bytes = data_.size() * sizeof(uint32_t);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_in_bytes * sizeof(uint32_t),
               data_.data(), GL_STATIC_DRAW);
}

#pragma mark - VertexPainter

VertexPainter::VertexPainter(const SharedVertexBuffer& vbo,
                             const SharedElementBuffer& ebo)
    : vbo_(vbo), ebo_(ebo) {
  glGenVertexArrays(1, &id_);
}

VertexPainter::~VertexPainter() {
  glDeleteVertexArrays(1, &id_);
}

void VertexPainter::Submit() const {
  glBindVertexArray(id_);
  if (vbo_) {
    vbo_->Submit();
  }
  if (ebo_) {
    ebo_->Submit();
  }

  glBindVertexArray(0);
  if (ebo_) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

void VertexPainter::DrawContents() const {
  if (ebo_) {
    glBindVertexArray(id_);
    glDrawElements(GL_TRIANGLES, ebo_->GetSize(), GL_UNSIGNED_INT,
                   reinterpret_cast<void*>(0));
    glBindVertexArray(0);
    return;
  }

  if (vbo_) {
    glBindVertexArray(id_);
    glDrawArrays(GL_TRIANGLES, 0, vbo_->GetSize());
    glBindVertexArray(0);
    return;
  }
}
