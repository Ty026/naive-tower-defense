#pragma once

#include <cstdint>
class Color {
public:
  constexpr Color() : r(0), g(0), b(0), a(0) {}
  constexpr Color(uint8_t c) : r(c), g(c), b(c), a(255) {}
  constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
  uint8_t r, g, b, a;

  static const Color Black;
  static const Color White;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Gray;
  static const Color Volcano;
  static const Color Orange;
  static const Color Gold;
  static const Color Yellow;
  static const Color Lime;
  static const Color Cyan;
  static const Color Purple;
  static const Color Magenta;
  static const Color Transparent;
  static Color       FromRGBA(uint32_t rgba);
  static Color       FromRGB(uint32_t rgb);
};

inline constexpr Color Color::Black{0, 0, 0};
inline constexpr Color Color::White{255, 255, 255};
inline constexpr Color Color::Red{245, 34, 45};
inline constexpr Color Color::Green{82, 196, 26};
inline constexpr Color Color::Blue{22, 119, 255};
inline constexpr Color Color::Gray{191, 191, 191};
inline constexpr Color Color::Volcano{250, 84, 28};
inline constexpr Color Color::Orange{250, 140, 22};
inline constexpr Color Color::Gold{250, 173, 20};
inline constexpr Color Color::Yellow{250, 219, 20};
inline constexpr Color Color::Lime{160, 217, 17};
inline constexpr Color Color::Cyan{19, 194, 194};
inline constexpr Color Color::Purple{114, 46, 209};
inline constexpr Color Color::Magenta{235, 47, 150};
inline constexpr Color Color::Transparent{};
