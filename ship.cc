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

namespace bship {
  Ship::Ship(ShipType type, int x, int y, Orientation orient) : x(x), y(y),type(type),orientation(orient) {}
  
  void Ship::render() {
    DrawRectanglePro((Rectangle){positionLUT[this->x][this->y][(int)gameState.us].x+3,positionLUT[this->x][this->y][(int)gameState.us].y+3,static_cast<float>(this->type != shipDestroyer ? 22*(int)this->type : 22*2),18},(Vector2){24/2,18/2},orientationToRotation(this->orientation),GRAY);

  }

  std::string orientationToString(Orientation orientation)
  {
    return orientation == orientationUp    ? "Up"    :
           orientation == orientationRight ? "Right" : 
           orientation == orientationDown  ? "Down"  :
           orientation == orientationLeft  ? "Left"  : "Unknown";
  }

  float orientationToRotation(Orientation orientation)
  {
    return orientation == orientationUp    ? -90    :
           orientation == orientationRight ? 0 : 
           orientation == orientationDown  ? 90  :
           orientation == orientationLeft  ? 180  : 0;
  }

  void renderArbitrayBoat(ShipType type, int x, int y, Orientation orientation)
  {
    DrawRectanglePro({static_cast<float>(x),static_cast<float>(y),static_cast<float>(type != shipDestroyer ? 22*(int)type : 22*2),18},(Vector2){24/2,18/2},orientationToRotation(orientation),GRAY);
  }
}
