/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/17.
 */

#include <glad/glad.h>
#include <memory>
#include <string>
#include <vector>

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
using AttributeList = std::vector<Attribute>;

#pragma mark - Buffer

template <typename T = float>
class Buffer {
 public:
  explicit Buffer(const std::vector<T>& data) : data_(data) {}
  virtual ~Buffer() = default;

  virtual uint32_t GetId() const final { return id_; }
  virtual int GetSize() const { return data_.size(); }
  virtual void Submit() const = 0;

 protected:
  const std::vector<T> data_;
  GLuint id_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Buffer);
};

#pragma mark - VertexBuffer

class VertexBuffer : public Buffer<float> {
 public:
  using SharedVertexBuffer = std::shared_ptr<VertexBuffer>;

  MAKE_SHARED_CONSTRUCTOR(VertexBuffer);
  ~VertexBuffer() override;

  AttributeList GetAttributeList() const { return attribute_list_; }
  void SetAttributeList(const AttributeList& attribute_list);

  int GetSize() const override { return data_.size() / one_vertex_size_; }

  void Submit() const override;

 protected:
  explicit VertexBuffer(const std::vector<float>& data,
                        const AttributeList& attribute_list = {});

 private:
  void UpdateOneVertexSize();

  AttributeList attribute_list_;
  int one_vertex_size_;
};
using SharedVertexBuffer = VertexBuffer::SharedVertexBuffer;

#pragma mark - IndexBuffer

class IndexBuffer : public Buffer<uint32_t> {
 public:
  using SharedElementBuffer = std::shared_ptr<IndexBuffer>;

  MAKE_SHARED_CONSTRUCTOR(IndexBuffer);
  ~IndexBuffer() override;

  void Submit() const override;

 protected:
  explicit IndexBuffer(const std::vector<uint32_t>& data);
};
using SharedElementBuffer = IndexBuffer::SharedElementBuffer;

#pragma mark - VertexPainter

class VertexPainter {
 public:
  using SharedVertexPainter = std::shared_ptr<VertexPainter>;

  MAKE_SHARED_CONSTRUCTOR(VertexPainter);
  ~VertexPainter();

  uint32_t GetId() const { return id_; }
  void Submit() const;
  void DrawContents() const;

 protected:
  explicit VertexPainter(const SharedVertexBuffer& vbo,
                         const SharedElementBuffer& ebo = nullptr);

 private:
  GLuint id_;
  const SharedVertexBuffer vbo_;
  const SharedElementBuffer ebo_;
};
using SharedVertexPainter = VertexPainter::SharedVertexPainter;
