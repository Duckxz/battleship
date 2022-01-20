#include "common.hh"

bship::GameEvent::GameEvent(EventType type, Event evt) : type(type), evt(std::make_unique<Event>(evt))  {}

void bship::GameEvent::handle()
{
  gameState.stateMtx.lock();
  switch(this->type)
  {
    case eventAttack:
      break;
    case eventHit:
      explosion->playing = true;
      explosion->currentFrame = 0;
      explosion->x = this->evt->data.hit.x;
      explosion->y = this->evt->data.hit.y;
      pageManager.toggle("explosion_oneshot");
      gameState.board[this->evt->data.hit.x-1][this->evt->data.hit.y-1][this->evt->data.hit.player] = true;
      break;
    case eventMiss:
      break;
    case eventShipPlacement:
      gameState.shipManager.place((ShipType)this->evt->data.ship.type,(int)this->evt->data.ship.x,(int)this->evt->data.ship.y,(Orientation)this->evt->data.ship.orientation);
      break;
    case eventTurn:
      gameState.myTurn = this->evt->data.turn.player == gameState.us ? true : false;
      break;
    case eventPlayerJoin:
      if (!gameState.connected && this->evt->data.join.success == 1)
      {
        gameState.connected = true;
        gameState.us = this->evt->data.join.player;
      }
      else
      {
        gameState.opponentConnected = true;
      }
      break;
    case eventPlayerLeave:
      if (this->evt->data.leave.player != gameState.us && gameState.opponentConnected)
      {
        gameState.opponentConnected = false;
      }
      else if (this->evt->data.leave.player == gameState.us)
      {
        gameState.connected = false;
        gameState.us = 2;
        Event emptyEvent = {0};
        // the client does a full reset to default state when we get force disconnected
        GameEvent fullReset = GameEvent(eventFullReset,emptyEvent);
        fullReset.handle();
      }
      break;
    case eventPlayerWin:
      if (this->evt->data.win.player == gameState.us)
      {
        gameState.won = true;
      }
      else
      {
        gameState.lost = true;
      }
      break;
    case eventReset:
      gameState.won = false;
      gameState.lost = false;
      for (int x = 0; x < NUMCELLS; x++)
      {
        for (int y = 0; y < NUMCELLS; y++)
        {
          gameState.board[x][y][0] = false;
          gameState.board[x][y][1] = false;
        }
      }
      gameState.allPlaced = false;
      gameState.myTurn = false;
      break;
    case eventFullReset:
      gameState.us = 2;
      gameState.won = false;
      gameState.lost = false;
      gameState.allPlaced = false;
      for (int x = 0; x < NUMCELLS; x++)
      {
        for (int y = 0; y < NUMCELLS; y++)
        {
          gameState.board[x][y][0] = false;
          gameState.board[x][y][1] = false;
        }
      }
      gameState.opponentConnected = false;
      gameState.connected = false;
      gameState.manualPlacementAllowed = true;
      gameState.myTurn = false;
      gameState.shipManager.ships.clear();
      break;
    default:
      break;
  }
  gameState.stateMtx.unlock();
}
