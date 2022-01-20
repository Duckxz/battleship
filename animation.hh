#pragma once

#include "common.hh"

namespace bship {
  class Animation : public IRenderable {
    private:
      Texture2D _tex;
    public:
      std::string name;
      bool playing;
      int frameCount;
      int fps;
      int width,height;
      int currentFrame;
      int x,y;
      std::chrono::time_point<std::chrono::high_resolution_clock> lastAnimTick;

      Animation(std::string name, std::string path,int fps, int frameCount, int width, int height);
      ~Animation();
      void tickAnim();
      void render();
  };

}
