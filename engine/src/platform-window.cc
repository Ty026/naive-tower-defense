#include <cstdlib>
#define GLFW_INCLUDE_NONE
#include "platform-window.h"
#include <GLFW/glfw3.h>
#include "gl.h"

Window::Window(std::string_view title, int w, int h) {
  if (glfwPlatformSupported(GLFW_PLATFORM_WAYLAND))
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

  glfwInit();
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow *window = glfwCreateWindow(w, h, title.data(), nullptr, nullptr);
  raw_window_ = window;
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, this);
  glfwSetWindowCloseCallback(window, [](GLFWwindow *p_window) {
    Event e{Event::Type::WindowClose};
    static_cast<Window *>(glfwGetWindowUserPointer(p_window))->event_handler_(e);
  });

  if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) exit(1);
}

void Window::PollEvents() {
  glfwPollEvents();
}

void Window::Update() {
  glfwSwapBuffers(static_cast<GLFWwindow *>(raw_window_));
}
