#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
// #define STBI_NO_STDIO
#include "image.h"
#include <stb_image.h>
#include "log.h"
Ref<Image> ImageLoader::operator()(std::string_view path) {
  int      w, h, channels;
  uint8_t *data = stbi_load(path.data(), &w, &h, &channels, 0);
  auto     error = stbi_failure_reason();
  if (error) {
    Log::Error("Error loading image '{}': '{}'", path, error);
    if (data) stbi_image_free(data);
    return nullptr;
  }
  Ref<Image> ret(new Image, [](Image *ptr) {
    stbi_image_free(ptr->data);
    ptr->data = nullptr;
  });
  ret->w = w;
  ret->h = h;
  ret->data = data;
  ret->channels = channels;
  return ret;
}
