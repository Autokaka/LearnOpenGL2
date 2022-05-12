/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/10.
 */

#pragma once

#include <glm/glm.hpp>

class Camera final {
 public:
  Camera();

  // position
  glm::vec3 GetPosition() const;
  void SetPosition(const glm::vec3& position);
  void GoForward(float step);
  void GoBackward(float step);
  void GoLeft(float step);
  void GoRight(float step);
  void GoUp(float step);
  void GoDown(float step);

  // rotation
  glm::mat4 GetRotation() const;
  void SetRotation(const glm::mat4& rotation);
  void LookAt(const glm::vec3& world_target, const glm::vec3& world_up);
  void Yaw(float degrees);
  void Pitch(float degrees);
  void Roll(float degrees);
  void PitchUp(float step_degrees);
  void PitchDown(float step_degrees);
  void YawLeft(float step_degrees);
  void YawRight(float step_degrees);
  void RollLeft(float step_degrees);
  void RollRight(float step_degrees);

  glm::mat4 GetTransformation() const;

 private:
  glm::mat4 camera_rotation_;
  glm::vec3 camera_position_;

  float local_x_rotate_degrees_;
  float local_y_rotate_degrees_;
  float local_z_rotate_degrees_;
  glm::vec3 local_position_;

  glm::mat4 GetCameraTranslateMatrix() const;
  glm::mat4 GetCameraRotateMatrix() const;
  glm::mat4 GetCameraTransformMatrix() const;

  glm::mat4 GetLocalTranslateMatrix() const;
  glm::mat4 GetLocalRotateMatrix() const;
  glm::mat4 GetLocalTransformMatrix() const;
};
