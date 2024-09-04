#pragma once

#include <vector>
#include <format>

class Log {
public:
  template <typename... Args>
  static void Info(std::format_string<Args...> fmt, Args &&...args);

  template <typename... Args>
  static void Trace(std::format_string<Args...> fmt, Args &&...args);

  template <typename... Args>
  static void Warn(std::format_string<Args...> fmt, Args &&...args);

  template <typename... Args>
  static void Error(std::format_string<Args...> fmt, Args &&...args);

private:
  static std::vector<char> log_buffer_;
};

template <typename... Args>
void Log::Info(std::format_string<Args...> fmt, Args &&...args) {
  log_buffer_.clear();
  std::format_to(std::back_inserter(log_buffer_), fmt, std::forward<Args>(args)...);
  log_buffer_.push_back('\0');
  printf("[Info] %s\n", log_buffer_.data());
}

template <typename... Args>
void Log::Trace(std::format_string<Args...> fmt, Args &&...args) {
  log_buffer_.clear();
  std::format_to(std::back_inserter(log_buffer_), fmt, std::forward<Args>(args)...);
  log_buffer_.push_back('\0');
  printf("[Trace] %s\n", log_buffer_.data());
}

template <typename... Args>
void Log::Warn(std::format_string<Args...> fmt, Args &&...args) {
  log_buffer_.clear();
  std::format_to(std::back_inserter(log_buffer_), fmt, std::forward<Args>(args)...);
  log_buffer_.push_back('\0');
  printf("[Warn] %s\n", log_buffer_.data());
}

template <typename... Args>
void Log::Error(std::format_string<Args...> fmt, Args &&...args) {
  log_buffer_.clear();
  std::format_to(std::back_inserter(log_buffer_), fmt, std::forward<Args>(args)...);
  log_buffer_.push_back('\0');
  printf("[Error] %s\n", log_buffer_.data());
}
