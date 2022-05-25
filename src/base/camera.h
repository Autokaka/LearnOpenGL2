/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/10.
 */

#pragma once

#include <glm/glm.hpp>

// TODO(Autokaka): Bring back Camera FPS mode.

enum class CameraMode {
  kFPS,
  kFly,
};

class Camera final {
 public:
  explicit Camera(const CameraMode& mode = CameraMode::kFly);

  CameraMode mode;

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
  void PitchUp(float step_degrees);
  void PitchDown(float step_degrees);
  void YawLeft(float step_degrees);
  void YawRight(float step_degrees);

  glm::mat4 GetTransformMatrix() const;
  glm::mat4 GetViewMatrix() const;

 private:
  glm::vec3 camera_x_;
  glm::vec3 camera_y_;
  glm::vec3 camera_z_;
  glm::vec3 camera_position_;

  float local_pitch_;
  float local_yaw_;
};
