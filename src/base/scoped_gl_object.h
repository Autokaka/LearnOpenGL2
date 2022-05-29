#pragma once

#include <functional>
#include <memory>

#include "macros.h"

class ScopedGLObject final {
 public:
  using Constructor = std::function<uint32_t()>;
  using Deleter = std::function<void(uint32_t id)>;

  MAKE_SHARED_CONSTRUCTOR(ScopedGLObject, Create);
  ~ScopedGLObject() {
    if (deleter_) {
      deleter_(id_);
    }
  }

  uint32_t GetId() const { return id_; }

 private:
  const uint32_t id_;
  const Deleter deleter_;

  explicit ScopedGLObject(const Constructor& constructor, const Deleter& deleter)
      : id_(constructor()), deleter_(deleter) {}

  DISALLOW_COPY_ASSIGN_AND_MOVE(ScopedGLObject);
};
using SharedGLObject = std::shared_ptr<ScopedGLObject>;
