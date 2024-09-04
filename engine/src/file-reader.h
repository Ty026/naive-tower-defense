#pragma once

#include "common.h"
#include "readable-stream.h"
#include <cstdio>
#include <string_view>
class FileReader : public ReadableStream {
public:
  [[nodiscard]] bool Open(std::string_view filename);
  size_t             Read(void *data, size_t len) override;
  size_t             Seek(size_t offset) override;
  size_t             Tell() override;
  size_t             size() override;

private:
  struct FileCloser {
    void operator()(std::FILE *f) { fclose(f); }
  };
  Scope<std::FILE, FileCloser> file_;
};
