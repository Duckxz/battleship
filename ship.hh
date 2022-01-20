#pragma once

#include "common.hh"

namespace bship {
  enum Orientation {
    orientationUp = 0,
    orientationDown,
    orientationLeft,
    orientationRight
  };
  
  enum ShipType {
    shipCarrier = 5,
    shipBattleship = 4,
    shipCruiser = 3,
    shipSubmarine = 2,
    shipDestroyer = 1
  };

  class Ship : public IRenderable {
    public:
      int x,y;
      ShipType type;
      Orientation orientation;
      Ship(ShipType,int,int,Orientation);
      void render();
  };
  
  void renderArbitrayBoat(ShipType,int,int,Orientation);
  std::string orientationToString(Orientation);
  float orientationToRotation(Orientation);
}

