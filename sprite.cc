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
