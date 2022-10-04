/*
 * Copyright (C) 2021 crton
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
