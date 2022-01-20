#pragma once
#include "common.hh" 

namespace bship {
  enum GameScreen {
    mainMenu = 0,
    connecting,
    ingame
  };

  class State {
    public:
#ifdef __ANDROID__
      struct android_app* app;
#endif
      std::mutex stateMtx;
      std::mutex inputMtx;
      InputHandler inputHandler;
      char us;
      int scoreP1,scoreP2;
      bool opponentConnected;
      bool won;
      bool lost; 
      bool allPlaced; // true if all boats have been placed
      bool manualPlacementAllowed; // true until we receive an event_ship from the server
      bool myTurn; // handled by server
      bool connected;
      char board[NUMCELLS][NUMCELLS][2]; // for the server board 0 is p1 and 1 is p2
      ShipManager shipManager;
      State();
  };
}

