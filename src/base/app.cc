/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/21.
 */

#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>

#include "app.h"

SharedApp App::singleton_ = nullptr;

SharedApp App::GetInstance() {
  if (!singleton_) {
    singleton_ = std::shared_ptr<App>(new App());
  }
  return singleton_;
}

App::App()
    : delegate_(nullptr),
      window_(nullptr),
      window_title_("Untitled"),
      window_size_(glm::vec2(800, 600)) {}

App::~App() {
  if (window_) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
  glfwTerminate();
}

void App::SetWindowTitle(const std::string& window_title) {
  glfwSetWindowTitle(window_, window_title.c_str());
  window_title_ = window_title;
}

int App::Run() {
  glfwSetErrorCallback([](int error, const char* description) {
    const auto delegate = App::GetInstance()->delegate_;
    if (delegate) {
      delegate->AppThrowsError(description);
    }
  });

  // create an opengl window via glfw.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window_ = glfwCreateWindow(window_size_.x, window_size_.y,
                             window_title_.c_str(), nullptr, nullptr);
  if (!window_) {
    return -1;
  }
  glfwMakeContextCurrent(window_);

  // initialize glad
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    if (delegate_) {
      delegate_->AppThrowsError("Failed to initialize GLAD");
    }
    return -1;
  }

  // initialize imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  // initialize viewport
  glfwSetFramebufferSizeCallback(
      window_, [](GLFWwindow* window, int width, int height) {
        const auto delegate = App::GetInstance()->delegate_;
        if (delegate) {
          delegate->WindowSizeDidChange(width, height);
        }
      });
  int viewport_width, viewport_height;
  glfwGetFramebufferSize(window_, &viewport_width, &viewport_height);
  if (delegate_) {
    delegate_->WindowSizeDidChange(viewport_width, viewport_height);
  }

  if (delegate_) {
    delegate_->WindowWillAppear();
  }

  while (!glfwWindowShouldClose(window_)) {
    if (delegate_) {
      delegate_->KeyboardEvent([&](int request_key) -> int {
        return glfwGetKey(window_, request_key);
      });
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (delegate_) {
      delegate_->DrawContent();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwPollEvents();
    glfwSwapBuffers(window_);
  }

  return 0;
}

void App::Exit(int code) {
  if (window_) {
    glfwSetWindowShouldClose(window_, true);
  }
}
