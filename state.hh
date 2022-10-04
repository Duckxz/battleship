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

