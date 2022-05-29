/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/29.
 */

#include <any>
#include <glm/glm.hpp>

#include "texture.h"

namespace {

template <typename T, typename U>
struct decay_equiv final : std::is_same<typename std::decay<T>::type, U>::type {};

template <typename T>
bool IsBool(const T& v) {
  return decay_equiv<decltype(v), bool>::value;
}

template <typename T>
bool IsInt(const T& v) {
  return decay_equiv<decltype(v), int>::value;
}

template <typename T>
bool IsFloat(const T& v) {
  return decay_equiv<decltype(v), float>::value;
}

template <typename T>
bool IsVec2(const T& v) {
  return decay_equiv<decltype(v), glm::vec2>::value;
}

template <typename T>
bool IsVec3(const T& v) {
  return decay_equiv<decltype(v), glm::vec3>::value;
}

template <typename T>
bool IsVec4(const T& v) {
  return decay_equiv<decltype(v), glm::vec4>::value;
}

template <typename T>
bool IsMat2(const T& v) {
  return decay_equiv<decltype(v), glm::mat2>::value;
}

template <typename T>
bool IsMat3(const T& v) {
  return decay_equiv<decltype(v), glm::mat3>::value;
}

template <typename T>
bool IsMat4(const T& v) {
  return decay_equiv<decltype(v), glm::mat4>::value;
}

template <typename T>
bool IsSampler2D(const T& v) {
  return decay_equiv<decltype(v), SharedTexture>::value;
}

}  // namespace
