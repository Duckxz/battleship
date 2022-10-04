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
  class ShipManager {
    public:
      bool shadowBoard[NUMCELLS][NUMCELLS][2];
#ifdef __ANDROID__
      Orientation androidShipOrientation;
#endif
      bool hasCarrier,hasBattleship,hasCruiser,hasSubmarine,hasDestroyer = false;
      std::vector<std::unique_ptr<Ship>> ships;
      ShipType selectedShip;
      Orientation orientationWanted;
      bool hasSelected;
      bool canPlace(ShipType,int,int,Orientation);
      bool place(ShipType,int,int,Orientation);
 
      ShipManager();
  };
}
