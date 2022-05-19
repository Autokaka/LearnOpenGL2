/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/17.
 */

#pragma once

#include <glad/glad.h>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "base/scoped_gl_object.h"
#include "macros.h"

#pragma mark - Attribute

enum class AttributeType {
  kCustom = 0,
  kFloat = 1,
  kVec2 = 2,
  kVec3 = 3,
  kVec4 = 4,
  kMat2 = 4,
  kMat3 = 9,
  kMat4 = 16,
};

class Attribute final {
 public:
  Attribute(const std::string& name, AttributeType type, int size = 0);

  std::string GetName() const { return name_; }
  AttributeType GetType() const { return type_; }
  int GetSize() const { return size_; }

 private:
  std::string name_;
  AttributeType type_;
  int size_;
};

#pragma mark - GLVertexBuffer

class GLVertexBuffer final
    : public std::enable_shared_from_this<GLVertexBuffer> {
 public:
  MAKE_SHARED_CONSTRUCTOR(GLVertexBuffer, Create);

  std::optional<uint32_t> GetVertexInfo() const;
  std::optional<uint32_t> GetDrawSequence() const;

 private:
  const SharedGLObject vertex_info_;
  const SharedGLObject draw_sequence_;

  explicit GLVertexBuffer(const SharedGLObject& vertex_info = nullptr,
                          const SharedGLObject& draw_sequence = nullptr);

  DISALLOW_COPY_ASSIGN_AND_MOVE(GLVertexBuffer);
};
using SharedGLVertexBuffer = std::shared_ptr<GLVertexBuffer>;

#pragma mark - VertexBuffer

class VertexBuffer final : std::enable_shared_from_this<VertexBuffer> {
 public:
  using SharedVertexBuffer = std::shared_ptr<VertexBuffer>;
  using VertexInfo = std::vector<float>;
  using AttributeList = std::vector<Attribute>;
  using DrawSequence = std::vector<uint32_t>;

  MAKE_SHARED_CONSTRUCTOR(VertexBuffer, Create);

  bool IsEmpty() const { return vertex_info_.empty(); }

  const VertexInfo& GetVertexInfo() const { return vertex_info_; }
  void SetVertexInfo(const VertexInfo& vertex_info);
  int GetVertexCount() const { return vertex_info_.size() / one_vertex_size_; }

  const AttributeList& GetAttributeList() const { return attribute_list_; }
  void SetAttributeList(const AttributeList& attribute_list);

  const DrawSequence& GetDrawSequence() const { return draw_sequence_; }
  void SetDrawSequence(const DrawSequence& draw_sequence);

  SharedGLVertexBuffer CreateGLObject() const;

 private:
  int one_vertex_size_;
  VertexInfo vertex_info_;
  AttributeList attribute_list_;
  DrawSequence draw_sequence_;

  explicit VertexBuffer(const VertexInfo& vertex_info = {},
                        const AttributeList& attribute_list = {},
                        const DrawSequence& draw_sequence = {});

  DISALLOW_COPY_ASSIGN_AND_MOVE(VertexBuffer);
};
using SharedVertexBuffer = VertexBuffer::SharedVertexBuffer;
