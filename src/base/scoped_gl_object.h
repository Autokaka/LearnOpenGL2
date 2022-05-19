#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include "macros.h"

class ScopedGLObject final {
 public:
  using SharedGLObject = std::shared_ptr<ScopedGLObject>;
  using Deleter = std::function<void(uint32_t id)>;

  static SharedGLObject Create(uint32_t id, const Deleter& deleter) {
    return std::shared_ptr<ScopedGLObject>(new ScopedGLObject(id, deleter));
  }
  ~ScopedGLObject() {
    if (deleter_) {
      deleter_(id_);
    }
  }

  uint32_t GetId() const { return id_; }

 private:
  const uint32_t id_;
  const Deleter deleter_;

  explicit ScopedGLObject(uint32_t id, const Deleter& deleter)
      : id_(id), deleter_(deleter) {}

  DISALLOW_COPY_ASSIGN_AND_MOVE(ScopedGLObject);
};
using SharedGLObject = ScopedGLObject::SharedGLObject;
