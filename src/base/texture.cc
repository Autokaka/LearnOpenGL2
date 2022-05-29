/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/23.
 */

#include <stb/stb_image.h>

#include "texture.h"

SharedTexture Texture::CreateFromFile(const std::string_view& file_path) {
  stbi_set_flip_vertically_on_load(true);

  int nr_components;
  int width = 0, height = 0;
  unsigned char* data =
      stbi_load(file_path.data(), &width, &height, &nr_components, 0);
  if (data) {
    const auto instance = SharedTexture(new Texture());
    instance->data_ = data;
    instance->width_ = width;
    instance->height_ = height;
    switch (nr_components) {
      case 3:
        instance->format_ = Format::kRGB;
        break;
      case 4:
        instance->format_ = Format::kRGBA;
        break;
      default:
        return nullptr;
    }
    return instance;
  }
  return nullptr;
}

Texture::Texture()
    : width_(0),
      height_(0),
      s_wrap_mode_(WrapMode::kClampToBorder),
      t_wrap_mode_(WrapMode::kClampToBorder),
      min_filter_(MinFilter::kLinearMipmapLinear),
      mag_filter_(MagFilter::kLinear),
      data_(nullptr) {}

Texture::~Texture() {
  if (data_) {
    stbi_image_free(data_);
    data_ = nullptr;
  }
}

void Texture::SetSWrapMode(const WrapMode& s_wrap_mode) {
  s_wrap_mode_ = s_wrap_mode;
  ReplaceLastCommand("SetSWrapMode",
                     [s_wrap_mode](const SharedGLObject gl_object) {
                       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                       static_cast<GLenum>(s_wrap_mode));
                     });
}

void Texture::SetTWrapMode(const WrapMode& t_wrap_mode) {
  t_wrap_mode_ = t_wrap_mode;
  ReplaceLastCommand("SetTWrapMode",
                     [t_wrap_mode](const SharedGLObject& gl_object) {
                       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                       static_cast<GLenum>(t_wrap_mode));
                     });
}

void Texture::SetMinFilter(const MinFilter& min_filter) {
  min_filter_ = min_filter;
  ReplaceLastCommand("SetMinFilter",
                     [min_filter](const SharedGLObject& gl_object) {
                       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                       static_cast<GLenum>(min_filter));
                     });
}

void Texture::SetMagFilter(const MagFilter& mag_filter) {
  mag_filter_ = mag_filter;
  ReplaceLastCommand("SetMagFilter",
                     [mag_filter](const SharedGLObject& gl_object) {
                       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                       static_cast<GLenum>(mag_filter));
                     });
}

SharedGLObject Texture::MakeGLObject() {
  if (gl_object_) {
    SubmitCommands();
    return gl_object_;
  }

  gl_object_ = ScopedGLObject::Create(
      []() -> GLuint {
        GLuint id;
        glGenTextures(1, &id);
        return id;
      },
      [](GLuint id) { glDeleteTextures(1, &id); });

  glBindTexture(GL_TEXTURE_2D, gl_object_->GetId());
  SetSWrapMode(s_wrap_mode_);
  SetTWrapMode(t_wrap_mode_);
  SetMinFilter(min_filter_);
  SetMagFilter(mag_filter_);
  glTexImage2D(/** define texture format in gl */ GL_TEXTURE_2D, 0,
               static_cast<GLenum>(format_), GetWidth(), GetHeight(), 0,
               /** define raw image format */ static_cast<GLenum>(format_),
               GL_UNSIGNED_BYTE, data_);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  return gl_object_ ? MakeGLObject() : nullptr;
}

void Texture::SubmitCommands() {
  if (gl_object_ && GetCommandList().size() > 0) {
    glBindTexture(GL_TEXTURE_2D, gl_object_->GetId());
    GPUAccess::SubmitCommands();
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}
