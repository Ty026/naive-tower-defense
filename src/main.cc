#include "engine.h"

int main() {
  auto engine = Engine::Initialize("Naive Tower Defense", 736, 544);
  engine->RunLoop();
  return 0;
}
