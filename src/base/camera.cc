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
  return camera_position_;
}

void Camera::SetPosition(const glm::vec3& position) {
  camera_position_ = position;
}

void Camera::GoForward(float step) {
  SetRotation(GetRotation());
  camera_position_ -= camera_z_ * step;
}

void Camera::GoBackward(float step) {
  GoForward(-step);
}

void Camera::GoLeft(float step) {
  SetRotation(GetRotation());
  camera_position_ -= camera_x_ * step;
}

void Camera::GoRight(float step) {
  GoLeft(-step);
}

void Camera::GoUp(float step) {
  SetRotation(GetRotation());
  camera_position_ += camera_y_ * step;
}

void Camera::GoDown(float step) {
  GoUp(-step);
}

#pragma mark - Rotation

glm::mat4 Camera::GetRotation() const {
  const auto local_rotation_x =
      glm::rotate(glm::mat4(1.0f), glm::radians(local_pitch_), {1, 0, 0});
  const auto local_rotation_y =
      glm::rotate(glm::mat4(1.0f), glm::radians(local_yaw_), {0, 1, 0});
  // clang-format off
  return glm::mat4 {
    camera_x_[0], camera_x_[1], camera_x_[2], 0,
    camera_y_[0], camera_y_[1], camera_y_[2], 0,
    camera_z_[0], camera_z_[1], camera_z_[2], 0,
               0,            0,            0, 1,
  } * local_rotation_x * local_rotation_y;
  // clang-format on
}

void Camera::SetRotation(const glm::mat4& rotation) {
  camera_x_ = glm::vec3(rotation[0]);
  camera_y_ = glm::vec3(rotation[1]);
  camera_z_ = glm::vec3(rotation[2]);
  local_pitch_ = 0;
  local_yaw_ = 0;
}

void Camera::LookAt(const glm::vec3& world_target, const glm::vec3& world_up) {
  camera_z_ = glm::normalize(GetPosition() - world_target);
  camera_x_ = glm::normalize(glm::cross(world_up, camera_z_));
  camera_y_ = glm::cross(camera_z_, camera_x_);
  local_pitch_ = 0;
  local_yaw_ = 0;
}

void Camera::Yaw(float degrees) {
  local_yaw_ = degrees;
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
  return glm::translate(glm::mat4(1.0f), GetPosition()) * GetRotation();
}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::transpose(GetRotation()) *
         glm::translate(glm::mat4(1.0f), -GetPosition());
}
