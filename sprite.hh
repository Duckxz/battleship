#pragma once

#include "common.hh"

namespace bship {
  class Sprite : public IRenderable {
    private:
      Texture2D _tex;
    public:
      std::string name;
      int x,y;
      Orientation orientation;
      float scale;
      int width,height;
      
      Sprite(std::string name, std::string path, int width, int height);
      ~Sprite();
      void render();
  };

}
