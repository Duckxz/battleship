#pragma once
#include "common.hh"

namespace bship {
  class ShipManager {
    public:
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
