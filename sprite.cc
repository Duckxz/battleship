#include "common.hh"
#include "animation.hh"

namespace bship {
  Sprite::Sprite(std::string name, std::string path, int width, int height)
  :
    name(name),
    orientation(orientationUp),
    scale(1.0f),
    width(width),
    height(height)
  {
    this->_tex = LoadTexture(path.c_str());
  }

  Sprite::~Sprite()
  {
    UnloadTexture(this->_tex);
  }

  void Sprite::render()
  {
    DrawTextureEx(this->_tex,(Vector2){(float)this->x,(float)this->y},orientationToRotation(this->orientation),this->scale,WHITE);
  }
}
