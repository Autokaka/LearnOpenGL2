/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/23.
 */

#pragma once

#include <glad/glad.h>
#include <list>
#include <mutex>

#include "gpu_access.h"
#include "scoped_gl_object.h"

class Texture : public GPUAccess<SharedGLObject>,
                std::enable_shared_from_this<Texture> {
 public:
  enum class WrapMode : GLenum {
    kRepeat = GL_REPEAT,
    kMirroredRepeat = GL_MIRRORED_REPEAT,
    kClampToEdge = GL_CLAMP_TO_EDGE,
    kClampToBorder = GL_CLAMP_TO_BORDER,
  };

  enum class MinFilter : GLenum {
    kNearest = GL_NEAREST,
    kLinear = GL_LINEAR,
    kNearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    kLinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    kNearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    kLinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
  };

  enum class MagFilter : GLenum {
    kNearest = GL_NEAREST,
    kLinear = GL_LINEAR,
  };

  using SharedTexture = std::shared_ptr<Texture>;

  static SharedTexture CreateFromFile(const std::string_view& file_path);
  ~Texture() override;

  WrapMode GetSWrapMode() const { return s_wrap_mode_; }
  void SetSWrapMode(const WrapMode& s_wrap_mode);
  WrapMode GetTWrapMode() const { return t_wrap_mode_; }
  void SetTWrapMode(const WrapMode& s_wrap_mode);
  MinFilter GetMinFilter() const { return min_filter_; }
  void SetMinFilter(const MinFilter& min_filter);
  MagFilter GetMagFilter() const { return mag_filter_; }
  void SetMagFilter(const MagFilter& mag_filter);
  int GetWidth() const { return width_; }
  int GetHeight() const { return height_; }
  int GetUnit() const { return unit_; }

  SharedGLObject CreateGLObject() override;

 private:
  static std::list<int> available_units_;
  static std::once_flag available_units_initilized_;
  int unit_;

  int width_;
  int height_;
  WrapMode s_wrap_mode_;
  WrapMode t_wrap_mode_;
  MinFilter min_filter_;
  MagFilter mag_filter_;
  unsigned char* data_;

  explicit Texture();

  DISALLOW_COPY_ASSIGN_AND_MOVE(Texture);
};
using SharedTexture = Texture::SharedTexture;
