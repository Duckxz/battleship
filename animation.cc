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


#include "common.hh"
#include "animation.hh"

namespace bship {
  Animation::Animation(std::string name, std::string path, int fps, int frameCount, int width, int height)
  :
    name(name),
    playing(true), 
    frameCount(frameCount),
    fps(fps),
    width(width),
    height(height)
  {
    this->_tex = LoadTexture(path.c_str());
    this->currentFrame = 0;
  }

  Animation::~Animation()
  {
    UnloadTexture(this->_tex);
  }

  /// this function ticks the animation, updating it's timings so that it draws at the required FPS
  void Animation::tickAnim()
  {
    // check this first to prevent unnecessary calculations
    if(!this->playing)
      return;

    float ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->lastAnimTick).count(); 
    bool shouldTick = ms >= 1000/this->fps;
    if (shouldTick == false)
    {
      return;
    }
    this->lastAnimTick = std::chrono::high_resolution_clock::now();
    this->currentFrame = this->currentFrame >= this->frameCount ? 0 : this->currentFrame + 1;
  }

  /// render the animation's current frame to the screen
  void Animation::render()
  {
    //std::cout << "drawing frame " << this->current_frame << " of anim '" << this->name << "' at (" << x << ", " << y << ") with rec [" << 0 << ", " << 8*current_frame << ", " << 8 << ", " << 8 << "]\n";
    DrawTextureRec(this->_tex,{0,float(this->width*currentFrame),(float)this->width,(float)this->height},{float(this->x),float(this->y)},WHITE);
  }
}
