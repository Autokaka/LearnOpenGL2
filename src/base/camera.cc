/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/10.
 */

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

#include "camera.h"

Camera::Camera(const CameraMode& mode) : mode(mode) {
  SetPosition(glm::vec3(0, 0, 3));
  LookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

#pragma mark - Position

glm::vec3 Camera::GetPosition() const {
  return GetTransformMatrix() * glm::vec4(0, 0, 0, 1);
}

void Camera::SetPosition(const glm::vec3& position) {
  camera_position_ = position;
  local_position_ = glm::vec3(0, 0, 0);
}

void Camera::GoForward(float step) {
  local_position_.z -= step;
  SetPosition(GetPosition());
}

void Camera::GoBackward(float step) {
  GoForward(-step);
}

void Camera::GoLeft(float step) {
  local_position_.x -= step;
  SetPosition(GetPosition());
}

void Camera::GoRight(float step) {
  GoLeft(-step);
}

void Camera::GoUp(float step) {
  local_position_.y += step;
  SetPosition(GetPosition());
}

void Camera::GoDown(float step) {
  GoUp(-step);
}

#pragma mark - Rotation

glm::mat4 Camera::GetRotation() const {
  return GetCameraRotateMatrix() * GetLocalTransformMatrix();
}

void Camera::SetRotation(const glm::mat4& rotation) {
  camera_rotation_ = rotation;
  local_pitch_ = 0;
  local_yaw_ = 0;
}

void Camera::LookAt(const glm::vec3& world_target, const glm::vec3& world_up) {
  SetRotation(glm::mat4(1.0f));
  const glm::vec3& camera_z = glm::normalize(GetPosition() - world_target);
  const glm::vec3& camera_x = glm::normalize(glm::cross(world_up, camera_z));
  const glm::vec3& camera_y = glm::cross(camera_z, camera_x);
  // clang-format off
  SetRotation({
    camera_x[0], camera_x[1], camera_x[2], 0,
    camera_y[0], camera_y[1], camera_y[2], 0,
    camera_z[0], camera_z[1], camera_z[2], 0,
              0,           0,           0, 1,
  });
  // clang-format on
}

void Camera::Yaw(float degrees) {
  local_yaw_ = -90 - degrees;
}

void Camera::Pitch(float degrees) {
  local_pitch_ = degrees;
}

void Camera::PitchUp(float step_degrees) {
  local_pitch_ += step_degrees;
}

void Camera::PitchDown(float step_degrees) {
  PitchUp(-step_degrees);
}

void Camera::YawLeft(float step_degrees) {
  local_yaw_ += step_degrees;
}

void Camera::YawRight(float step_degrees) {
  YawLeft(-step_degrees);
}

glm::mat4 Camera::GetTransformMatrix() const {
  return GetCameraTransformMatrix() * GetLocalTransformMatrix();
}

#pragma mark - CameraTrasformationUtil

glm::mat4 Camera::GetCameraTranslateMatrix() const {
  return glm::translate(glm::mat4(1.0f), camera_position_);
}

glm::mat4 Camera::GetCameraRotateMatrix() const {
  return camera_rotation_;
}

glm::mat4 Camera::GetCameraTransformMatrix() const {
  return GetCameraTranslateMatrix() * GetCameraRotateMatrix();
}

#pragma mark - LocalTransformationUtil

glm::mat4 Camera::GetLocalTransformMatrix() const {
  const glm::mat4 local_rotation_x = glm::rotate(
      glm::mat4(1.0f), glm::radians(local_pitch_), glm::vec3(1, 0, 0));
  const glm::mat4 local_rotation_y = glm::rotate(
      glm::mat4(1.0f), glm::radians(local_yaw_), glm::vec3(0, 1, 0));

  const glm::mat4 local_translation =
      glm::translate(glm::mat4(1.0f), local_position_);

  if (mode == CameraMode::kFly) {  // Fly mode
    return (local_rotation_x * local_rotation_y) * local_translation;
  } else {  // FPS mode
    return local_rotation_y * (local_translation * local_rotation_x);
  }
}
