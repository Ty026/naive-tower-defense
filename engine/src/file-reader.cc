#include "file-reader.h"
#include "log.h"

bool FileReader::Open(std::string_view filename) {
  file_.reset(fopen(filename.data(), "rb"));
  if (!file_) {
    Log::Error("Failed to open file '{}'", filename);
    return false;
  }
  return true;
}

size_t FileReader::Read(void *data, size_t len) {
  return fread(data, 1, len, file_.get());
}

size_t FileReader::Seek(size_t offset) {
  if (fseek(file_.get(), offset, SEEK_SET)) {
    Log::Error("Failed to seek in file");
    return 0;
  }
  return std::ftell(file_.get());
}

size_t FileReader::Tell() {
  return std::ftell(file_.get());
}

size_t FileReader::size() {
  auto pos = std::ftell(file_.get());
  std::fseek(file_.get(), 0, SEEK_END);
  auto size = std::ftell(file_.get());
  std::fseek(file_.get(), pos, SEEK_SET);
  return size;
}
