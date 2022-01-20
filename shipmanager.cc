#include "common.hh"

namespace bship {
  ShipManager::ShipManager()
  {
    this->orientationWanted = orientationDown;
#ifdef __ANDROID__
    this->androidShipOrientation = orientationUp;
#endif
  }
  
  // the origin of a ship is at the bottom
  bool ShipManager::canPlace(ShipType type,int x, int y, Orientation orientation)
  {
    /*int positions[12];
    positions[0] = x; // origin x
    positions[1] = y; // origin y
    positions[2] = (orientation == Left) ? x-1 : x+1; // first x
    positions[3] = (orientation == Up) ? y+1 : y-1; // first y
    positions[4] = (orientation == Left) ? x-2 : x+2; // second x
    positions[5] = (orientation == Up) ? y+2 : y-2; // second y
    if((int)type > 2) {
      positions[6]; // third x
      positions[7]; // third y
      positions[8]; // fourth x
      positions[9]; // fourth y
      positions[10]; // fifth x
      positions[11]; // fifth y
    }
    */
    int size = type != shipDestroyer ? (int)type - 1 : 1;
    int endX = orientation == orientationRight ? x+size :
               orientation == orientationLeft  ? x-size : 
               orientation != orientationRight && orientation != orientationLeft ? x : 666;
    int endY = orientation == orientationUp    ? y-size :
               orientation == orientationDown  ? y+size : 
               orientation != orientationUp && orientation != orientationDown ? y : 666;
    printf("checking wether boat with size %d and origin %d,%d and end %d,%d fits on board\n",size,x,y,endX,endY); 
    if (x >= 0 && x < NUMCELLS)
    {
      if (y >= 0 && y < NUMCELLS)
      {
        if (orientation == orientationUp)
        {
          if (y-size >= 0 && y-size < NUMCELLS)
          {
            return true;
          }
          else
          {
            printf("can't place: size causes boat to fall outside of board on y axis when facing up\n");
            return false;
          }
        }
        else if (orientation == orientationLeft)
        {
          if (x-size >= 0 && x-size < NUMCELLS)
          {
            return true;
          }
          else
          {
            printf("can't place: size causes boat to fall outside of board on x axis when facing left\n");
            return false;
          }
        }
        else if (orientation == orientationDown)
        {
          if (y+size >= 0 && y+size < NUMCELLS)
          {
            return true;
          }
          else
          {
            printf("can't place: size causes boat to fall outside of board on y axis when facing down\n");
            return false;
          }
        }
        else if (orientation == orientationRight)
        {
          if (x+size >= 0 && x+size < NUMCELLS)
          {
            return true;
          }
          else
          {
            printf("can't place: size causes boat to fall outside of board on x axis when facing right\n");
            return false;
          }
        }
      }
      else
      {
        printf("can't place: y falls outside of the board\n");
        return false;
      }
    }
    else
    {
      printf("can't place: x falls outside of the board\n");
      return false;
    }
    return false;
    /*return (x >= 0 && x <= NUMCELLS && y >= 0 && y <= NUMCELLS) ? 
             (orientation == orientationUp) ? 
               y-(int)type > 0 && y-(int)type <= NUMCELLS ? true : false :
             (orientation == orientationLeft) ?
               x-(int)type > 0 && x-(int)type <= NUMCELLS ? true : false :
             (orientation == orientationRight) ?
               x+(int)type > 0 && x+(int)type <= NUMCELLS ? true : false :
             (orientation == orientationDown) ?
               y+(int)type > 0 && y+(int)type <= NUMCELLS ? true : false : false : false; */
  }
  
  bool ShipManager::place(ShipType type,int x, int y, Orientation orientation)
  {
    if (gameState.allPlaced)
    {
      return false;
    }

    if ((this->hasCarrier && type == shipCarrier) || (this->hasBattleship && type == shipBattleship) || (this->hasCruiser && type == shipCruiser) || (this->hasSubmarine && type == shipSubmarine) || (this->hasDestroyer && type == shipDestroyer))
    {
      return false;
    }
    if (this->canPlace(type,x-1,y-1,orientation))
    {
#ifdef __ANDROID__
      this->ships.push_back(std::make_unique<Ship>(type,x,y,this->androidShipOrientation));
#else
      this->ships.push_back(std::make_unique<Ship>(type,x,y,orientation));
#endif
#ifdef __ANDROID__
      switch(this->androidShipOrientation)
#else
      switch (type)
#endif
      {
        case shipCarrier:
          this->hasCarrier = true;
          break;
        case shipBattleship:
          this->hasBattleship = true;
          break;
        case shipSubmarine:
          this->hasSubmarine = true;
          break;
        case shipCruiser:
          this->hasCruiser = true;
          break;
        case shipDestroyer:
          this->hasDestroyer = true;
          break;
      }
      if (this->hasCarrier && this->hasBattleship && this->hasSubmarine && this->hasCruiser && this->hasDestroyer)
      {
        gameState.allPlaced = true;
      }
      return true;
    }
    return false;
  }
}
