#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include "macros.h"

class ScopedGLObject;
using SharedGLObject = std::shared_ptr<ScopedGLObject>;

class ScopedGLObject {
 public:
  using Deleter = std::function<void(uint32_t id)>;

  static SharedGLObject MakeShared(uint32_t id, const Deleter& deleter) {
    return std::make_shared<ScopedGLObject>(id, deleter);
  }
  explicit ScopedGLObject(uint32_t id, const Deleter& deleter)
      : id_(id), deleter_(deleter) {}
  ~ScopedGLObject() { deleter_(id_); }

  uint32_t GetId() const { return id_; }

 protected:
  const uint32_t id_;

 private:
  const Deleter deleter_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(ScopedGLObject);
};
