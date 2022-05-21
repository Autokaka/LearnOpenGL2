/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/21.
 */

#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "macros.h"

class App : public std::enable_shared_from_this<App> {
 public:
  class Delegate {
   public:
    using KeyStateGetter = const std::function<int(int)>;

    virtual void AppThrowsError(const std::string& error) = 0;

    virtual void WindowWillAppear() = 0;
    virtual void WindowSizeDidChange(int width, int height) = 0;

    virtual void DrawContent() = 0;

    virtual void KeyboardEvent(const KeyStateGetter& key_state_getter) = 0;
  };

  using SharedApp = std::shared_ptr<App>;

  static SharedApp GetInstance();
  ~App();

  std::string GetWindowTitle() const { return window_title_; }
  void SetWindowTitle(const std::string& window_title);

  glm::vec2 GetWindowSize() const { return window_size_; };
  void SetWindowSize(const glm::vec2& window_size);

  int Run();
  void Exit(int code);

 protected:
  static SharedApp singleton_;

  Delegate* delegate_;

  App();

 private:
  GLFWwindow* window_;
  std::string window_title_;
  glm::vec2 window_size_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(App);
};
using SharedApp = App::SharedApp;
using AppDelegate = App::Delegate;
