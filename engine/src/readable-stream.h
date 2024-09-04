#pragma once
#include <cstddef>
class ReadableStream {
public:
  virtual ~ReadableStream() = default;
  virtual size_t Read(void *buf, size_t len) = 0;
  virtual size_t Seek(size_t offset) = 0;
  virtual size_t Tell() = 0;
  virtual size_t size() = 0;
};
