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
  /// values for orientation
  enum Orientation {
    orientationUp = 0,
    orientationDown,
    orientationLeft,
    orientationRight
  };
  
  /// values for ship types. their value is equal to their size except for shipDestroyer because shipDestroyer and shipSubmarine will have the same value and cause ambiguity
  enum ShipType {
    shipCarrier = 5,
    shipBattleship = 4,
    shipCruiser = 3,
    shipSubmarine = 2,
    shipDestroyer = 1
  };

  /// class to represent ships in the game
  class Ship : public IRenderable {
    public:
      int x,y;
      ShipType type;
      Orientation orientation;
      Ship(ShipType,int,int,Orientation);
      void render();
  };
  
  /// method to render an arbitrary boat to the screen
  void renderArbitrayBoat(ShipType,int,int,Orientation);
  /// method to convert an orientation enum value to a string, used by androidOrientationButton to display current orientation
  std::string orientationToString(Orientation);
  /// method to convert orientation to a degree of rotation for rendering
  float orientationToRotation(Orientation);
}

