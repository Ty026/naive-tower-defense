#include "log.h"
#include <cstdlib>
#define GLFW_INCLUDE_NONE
#include "platform-window.h"
#include <GLFW/glfw3.h>
#include "gl.h"

Window::Window(std::string_view title, int w, int h) : size_(w, h) {
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

  glfwGetWindowContentScale(window, &dpi_scale_, nullptr);
  // Wayland might not provide the correct content scale at window creation time.
  // To handle this, we listen for content scale changes
  // using glfwSetWindowContentScaleCallback
  // ensure we get the correct scale after the window has been fully initialized.
  glfwSetWindowContentScaleCallback(window, [](GLFWwindow *window, float x, float y) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    self->dpi_scale_ = x;
    auto e = Event{Event::Type::WindowResize};
    e.window.size = self->size();
    e.window.dpi_scale = x;
    self->event_handler_(e);
  });

  if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) exit(1);
}

void Window::PollEvents() {
  glfwPollEvents();
}

void Window::Update() {
  glfwSwapBuffers(static_cast<GLFWwindow *>(raw_window_));
}
