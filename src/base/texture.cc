/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/23.
 */

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "texture.h"

std::list<int> Texture::available_units_{};
std::once_flag Texture::available_units_initilized_;

SharedTexture Texture::CreateFromFile(const std::string_view& file_path) {
  int nr_channels;
  int width = 0, height = 0;
  unsigned char* data =
      stbi_load(file_path.data(), &width, &height, &nr_channels, 0);
  if (data) {
    const auto instance = SharedTexture(new Texture());
    instance->data_ = data;
    instance->width_ = width;
    instance->height_ = height;
    return instance;
  }
  return nullptr;
}

Texture::Texture()
    : unit_(-1),
      width_(0),
      height_(0),
      s_wrap_mode_(WrapMode::kClampToBorder),
      t_wrap_mode_(WrapMode::kClampToBorder),
      min_filter_(MinFilter::kLinearMipmapLinear),
      mag_filter_(MagFilter::kLinear),
      data_(nullptr) {
  std::call_once(available_units_initilized_, [&]() {
    for (int i = 0; i < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS; ++i) {
      available_units_.emplace_back(i);
    }
  });
}

Texture::~Texture() {
  if (data_) {
    stbi_image_free(data_);
    data_ = nullptr;
  }

  if (unit_ >= 0) {
    available_units_.emplace_back(unit_);
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

SharedGLObject Texture::CreateGLObject() {
  if (available_units_.empty()) {
    return nullptr;
  }

  unit_ = available_units_.front();
  available_units_.pop_front();
  auto gl_texture = ScopedGLObject::Create(
      []() -> GLuint {
        GLuint id;
        glGenTextures(1, &id);
        return id;
      },
      [](GLuint id) { glDeleteTextures(1, &id); });

  glActiveTexture(GL_TEXTURE0 + unit_);
  glBindTexture(GL_TEXTURE_2D, gl_texture->GetId());
  SetSWrapMode(s_wrap_mode_);
  SetTWrapMode(t_wrap_mode_);
  SetMinFilter(min_filter_);
  SetMagFilter(mag_filter_);
  SubmitCommands(gl_texture);
  glTexImage2D(/** define texture format in gl */ GL_TEXTURE_2D, 0, GL_RGB,
               GetWidth(), GetHeight(), 0,
               /** define raw image format */ GL_RGB, GL_UNSIGNED_BYTE, data_);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  return gl_texture;
}
