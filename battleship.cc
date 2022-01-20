#include "common.hh"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
using namespace bship;

int size = 26;
PageManager pageManager;
State gameState;
Animation* explosion; 
Vector2 positionLUT[NUMCELLS][NUMCELLS][3];

#ifdef __EMSCRIPTEN__
void drawEverything()
{
  Camera2D cam{};
  cam.target = {0,0};
  cam.zoom = 1.0f;
  cam.rotation = 0.0f;
  BeginDrawing();
    BeginMode2D(cam);
      pageManager.render();
      pageManager.renderUI();
    EndMode2D();
  EndDrawing();
  gameState.inputHandler.checkIboxes();
}
#endif

void computePositionLUT(int offsetX,int offsetY)
{
  for (int x = 0; x < NUMCELLS; x++)
  { 
    for (int y = 0; y < NUMCELLS; y++)
    {
      positionLUT[x][y][0] = {static_cast<float>((offsetX+x*size+size/2+2)-3),static_cast<float>(((offsetY-size*NUMCELLS-5)+y*size+size/2-5))};
      positionLUT[x][y][1] = {static_cast<float>((offsetX+x*size+size/2+2)-3),static_cast<float>((offsetY+y*size+size/2)+2)};
      positionLUT[x][y][2] = {-5.0f,-5.0f};
    }
  } 
}

int main()
{
  int w,h;
  int offsetX;
  int offsetY;
#ifndef __EMSCRIPTEN__
  Camera2D cam{};
	cam.target = {0,0};
  cam.zoom = 1.0f;
	cam.rotation = 0.0f;
#endif
  InitWindow(/*(size*size)*2,(size*size)*2*/670,580,"Battleship");
  w = GetScreenWidth();
	h = GetScreenHeight();
	offsetX = (w / 2) - (size*NUMCELLS/2);
	offsetY = (h / 2);
	if(!IsWindowReady()) {
		fprintf(stderr,"Failed to create raylib window\n");
		return -1;		
	}
  SetExitKey(KEY_NULL);
#ifdef __ANDROID__
  gameState.app = GetAndroidApp();
#endif
	// compute a LUT for all cell middle point vectors for both boards to easily fetch positions of cells given an x and y
  computePositionLUT(offsetX,offsetY);
#ifdef __ANDROID__
  ibox* androidOrientationButton;
#endif
  // TODO: make cells act based on gameState.us
  for (int x = 0; x < NUMCELLS; x++)
  {
    for (int y = 0; y < NUMCELLS; y++)
    {
      gameState.inputHandler.iboxes.push_back(new ibox{iboxCell,(Rectangle){positionLUT[x][y][0].x-size/2,positionLUT[x][y][0].y-size/2,(float)size,(float)size},[x,y]()
      {
        if (gameState.us == 0)
        {
          if (/*gameState.connected && gameState.opponentConnected && !gameState.allPlaced &&*/ gameState.shipManager.hasSelected && gameState.shipManager.canPlace(gameState.shipManager.selectedShip == shipDestroyer ? shipSubmarine : gameState.shipManager.selectedShip,x,y,gameState.shipManager.orientationWanted))
          {
            if (gameState.shipManager.place(gameState.shipManager.selectedShip,x+1,y+1,gameState.shipManager.orientationWanted))
            {
              gameState.shipManager.hasSelected = false;
              pageManager.toggle("boat_under_cursor",false);
            }
            else
            {
              printf("you can't place that boat here\n");
            }
          }
        }
        else if (gameState.us == 1)
        {
          if (gameState.connected && gameState.opponentConnected && gameState.allPlaced)
          {
            // TODO: send attack packet to NetSpooler protocol handler
          }
        }
        printf("cell %d,%d on top board pressed!\n",x+1,y+1);
      }});
      gameState.inputHandler.iboxes.push_back(new ibox{iboxCell,(Rectangle){positionLUT[x][y][1].x-size/2,positionLUT[x][y][1].y-size/2,(float)size,(float)size},[x,y]()
      {
        if (gameState.us == 1)
        {
          if (/*gameState.connected && gameState.opponentConnected && !gameState.allPlaced &&*/ gameState.shipManager.hasSelected && gameState.shipManager.canPlace(gameState.shipManager.selectedShip == shipDestroyer ? shipSubmarine : gameState.shipManager.selectedShip,x,y,gameState.shipManager.orientationWanted))
          {
            if (gameState.shipManager.place(gameState.shipManager.selectedShip,x+1,y+1,gameState.shipManager.orientationWanted))
            {
              gameState.shipManager.hasSelected = false;
              pageManager.toggle("boat_under_cursor",false);
            }
            else
            {
              printf("you can't place that boat here\n");
            }
          }
        }
        else if (gameState.us == 0)
        {
          if (gameState.connected && gameState.opponentConnected && gameState.allPlaced)
          {
            // TODO: send attack packet to NetSpooler protocol handler
          }
        }

        printf("cell %d,%d on bottom board pressed\n",x+1,y+1);
      }});
    }
  }

#ifdef __ANDROID__
  androidOrientationButton = new ibox{iboxUIButton,(Rectangle){positionLUT[9][2][0].x+size*3-size/2,positionLUT[9][2][0].y-size/2,(float)(size*5)-6,(float)size},[]()
  {
    gameState.shipManager.androidShipOrientation = gameState.shipManager.androidShipOrientation == orientationUp ? orientationRight   :
                                                    gameState.shipManager.androidShipOrientation == orientationRight ? orientationDown :
                                                    gameState.shipManager.androidShipOrientation == orientationDown ? orientationLeft  :
                                                    gameState.shipManager.androidShipOrientation == orientationLeft ? orientationUp : orientationUp;

  }};
  gameState.inputHandler.iboxes.push_back(androidOrientationButton);
#endif

  SetTargetFPS(25);
  
  explosion = new Animation("exp","assets/exp_sheet.png",10,9,16,16);
  explosion->x = positionLUT[0][0][0].x-explosion->width/2;
  explosion->y = positionLUT[0][0][0].y-explosion->height/2;
  ibox* selectCarrier = new ibox{iboxUIButton,(Rectangle){positionLUT[9][3][1].x+45-24/2,positionLUT[9][3][1].y-18/2,18.0f,static_cast<float>(22*(int)shipCarrier)},[]()
  {
    gameState.shipManager.selectedShip = shipCarrier;
    if (!gameState.shipManager.hasSelected)
    {
      gameState.shipManager.hasSelected = true;
      pageManager.toggle("boat_under_cursor",true);
    }
  }};
  ibox* selectBattleship = new ibox{iboxUIButton,(Rectangle){positionLUT[9][3][1].x+45+22-24/2,positionLUT[9][3][1].y-18/2,18.0f,static_cast<float>(22*(int)shipBattleship)},[]()
  {
    gameState.shipManager.selectedShip = shipBattleship;
    if (!gameState.shipManager.hasSelected)
    {
      gameState.shipManager.hasSelected = true;
      pageManager.toggle("boat_under_cursor",true);
    }
  }};
  ibox* selectCruiser = new ibox{iboxUIButton,(Rectangle){positionLUT[9][3][1].x+45+22*2-24/2,positionLUT[9][3][1].y-18/2,18.0f,static_cast<float>(22*(int)shipCruiser)},[]()
  {
    gameState.shipManager.selectedShip = shipCruiser;
    if (!gameState.shipManager.hasSelected)
    {
      gameState.shipManager.hasSelected = true;
      pageManager.toggle("boat_under_cursor",true);
    }
  }};
  ibox* selectSubmarine = new ibox{iboxUIButton,(Rectangle){positionLUT[9][3][1].x+45+22*3-24/2,positionLUT[9][3][1].y-18/2,18.0f,static_cast<float>(22*(int)shipSubmarine)},[]()
  {
    gameState.shipManager.selectedShip = shipSubmarine;
    if (!gameState.shipManager.hasSelected)
    {
      gameState.shipManager.hasSelected = true;
      pageManager.toggle("boat_under_cursor",true);
    }
  }};
  ibox* selectDestroyer = new ibox{iboxUIButton,(Rectangle){positionLUT[9][3][1].x+45+22*4-24/2,positionLUT[9][3][1].y-18/2,18.0f,static_cast<float>(22*2)},[]()
  {
    gameState.shipManager.selectedShip = shipDestroyer; // because enums
    if (!gameState.shipManager.hasSelected)
    {
      gameState.shipManager.hasSelected = true;
      pageManager.toggle("boat_under_cursor",true);
    }
  }};

  gameState.inputHandler.iboxes.push_back(selectCarrier);
  gameState.inputHandler.iboxes.push_back(selectBattleship);
  gameState.inputHandler.iboxes.push_back(selectCruiser);
  gameState.inputHandler.iboxes.push_back(selectSubmarine);
  gameState.inputHandler.iboxes.push_back(selectDestroyer);

  pageManager.make("board",false,true,[&offsetX,&offsetY]()
  {
    ClearBackground(BLACK);
    for (int t= 0; t < 2; t++)
    {
      if (!t) offsetY -= size * NUMCELLS + 10;
      if (t) offsetY +=  size * NUMCELLS + 10;
      for (int i = 0; i < NUMCELLS; i++)
      {
        DrawLine(offsetX+i * size, offsetY+0,offsetX+i * size, offsetY+size * NUMCELLS,GREEN);
        DrawLine(offsetX+0,offsetY+i * size, offsetX+size*NUMCELLS, offsetY+i * size,GREEN);
      }
      DrawLine(offsetX+size*NUMCELLS+1,offsetY+1+0,offsetX+size*NUMCELLS,offsetY+size*NUMCELLS,GREEN);
      DrawLine(offsetX+0,offsetY+size*NUMCELLS+1,offsetX+size*NUMCELLS,offsetY+size*NUMCELLS+1,GREEN);
    }
    char character[] = {65,0};
    char number[] = {48,0};
    for (int x = 0; x < NUMCELLS; x++)
    {
      DrawText(reinterpret_cast<char*>(&character),positionLUT[x][0][0].x,positionLUT[x][0][0].y-28,12,GREEN);
      DrawText(reinterpret_cast<char*>(&character),positionLUT[x][9][1].x,positionLUT[x][9][1].y+14,12,GREEN);
      character[0]++;
    }

    for (int y = 0; y < NUMCELLS; y++)
    {
      DrawText(reinterpret_cast<char*>(&number),positionLUT[0][y][0].x-24,positionLUT[0][y][0].y-7,12,GREEN);
      DrawText(reinterpret_cast<char*>(&number),positionLUT[0][y][1].x-24,positionLUT[0][y][1].y-7,12,GREEN);
      number[0]++;
    }
  });

  pageManager.make("boat_under_cursor",true,false,[]()
  {
    KeyboardKey lastKey = gameState.inputHandler.poll();
    if (!gameState.shipManager.hasSelected || (lastKey == KEY_BACKSPACE && gameState.shipManager.hasSelected))
    {
      if (lastKey == KEY_ESCAPE || lastKey == KEY_BACKSPACE)
      {
        gameState.shipManager.hasSelected = false;
      }
      pageManager.toggle("boat_under_cursor");
      return;
    }
    int resX,resY;
    bool mouseClicked = gameState.inputHandler.pollMouse(MOUSE_BUTTON_RIGHT,&resX,&resY);
    if (mouseClicked)
    {
      gameState.shipManager.orientationWanted = gameState.shipManager.orientationWanted == orientationUp    ? orientationRight :
                                                gameState.shipManager.orientationWanted == orientationRight ? orientationDown  :
                                                gameState.shipManager.orientationWanted == orientationDown  ? orientationLeft  :
                                                gameState.shipManager.orientationWanted == orientationLeft  ? orientationUp    : orientationUp;
 
    }
    int x = GetMouseX();
    int y = GetMouseY();
    renderArbitrayBoat(gameState.shipManager.selectedShip,x,y,gameState.shipManager.orientationWanted); 
  });

  pageManager.make("boats_for_placing",true,true,[]()
  {
    renderArbitrayBoat(shipCarrier,positionLUT[9][3][1].x+45,positionLUT[9][3][1].y,orientationDown);
    renderArbitrayBoat(shipBattleship,positionLUT[9][3][1].x+45+22,positionLUT[9][3][1].y,orientationDown);
    renderArbitrayBoat(shipCruiser,positionLUT[9][3][1].x+45+22*2,positionLUT[9][3][1].y,orientationDown);
    renderArbitrayBoat(shipSubmarine,positionLUT[9][3][1].x+45+22*3,positionLUT[9][3][1].y,orientationDown);
    renderArbitrayBoat(shipSubmarine,positionLUT[9][3][1].x+45+22*4,positionLUT[9][3][1].y,orientationDown);
  });

#ifdef __ANDROID__
  pageManager.make("android_draworientationbutton",true,true,[androidOrientationButton]()
  {
    char buttonString[40];
    snprintf(reinterpret_cast<char*>(&buttonString),40,"Orientation: %s",orientationToString(gameState.shipManager.androidShipOrientation).c_str());
    DrawRectangleLinesEx(androidOrientationButton->spot,2,GREEN);
    DrawText(reinterpret_cast<char*>(&buttonString),androidOrientationButton->spot.x+size/2,androidOrientationButton->spot.y+6,12,GREEN);
  });
#endif

  pageManager.make("debug_drawposlut",true,false,[]()
  {
    for (int x = 0; x < NUMCELLS; x++)
    {
      for (int y = 0; y < NUMCELLS; y++)
      {
        DrawPixel(positionLUT[x][y][0].x,positionLUT[x][y][0].y,RED);
        DrawPixel(positionLUT[x][y][1].x,positionLUT[x][y][0].y,RED);
      }
    }
  });

  pageManager.make("debug_drawiboxes",true,true,[]()
  {
    for (ibox* inputBox: gameState.inputHandler.iboxes)
    {
      DrawRectangleLinesEx(inputBox->spot,1,RED);
    }
  });
  pageManager.make("player_scores",true,true,[w]()
  {
    char p1[15];
    char p2[15];
    const char* infoP1 = gameState.us == 0 ? "you" : // we are player 1
                   gameState.us != 0 && !gameState.opponentConnected ? "?" : ""; // we aren't player 1 and player 1 is not connected 
    const char* infoP2 = (gameState.us == 1) ? "you" : // we are player 2
                   gameState.us != 1 && !gameState.opponentConnected ? "?" : ""; // we aren't player 2 and player 2 is not connected
    snprintf(reinterpret_cast<char*>(&p1),15,"P1 (%s)\n%d",infoP1,gameState.scoreP1);
    snprintf(reinterpret_cast<char*>(&p2),15,"P2 (%s)\n%d",infoP2,gameState.scoreP2);
    DrawText(p1,10,5,14,GREEN);
    DrawText(p2,gameState.us == 0 ? w-50 : w-65,5,14,GREEN);
  });
  pageManager.make("explosion_oneshot",true,true,[]()
  {
    if (explosion->currentFrame != explosion->frameCount)
    {
      explosion->render();
      explosion->tickAnim();
    }
    else 
    {
      explosion->currentFrame = 0;
      explosion->playing = false;
      pageManager.toggle("explosion_oneshot");
    }
  });

  pageManager.make("boats",true,true,[]()
  {
    for(std::unique_ptr<Ship>& ship : gameState.shipManager.ships) {
      /*DrawRectanglePro(
          (Rectangle) {positionLUT[ship->x-1][ship->y-1][static_cast<int>(gameState.us)].x+3,positionLUT[ship->x][ship->y][static_cast<int>(gameState.us)].y+3,static_cast<float>(22*static_cast<int>(ship->type)+size/2),18}, 
          (Vector2){24/2,18/2},
          orientationToRotation(ship->orientation),
          GRAY);*/
      //DrawRectanglePro((Rectangle){positionLUT[ship->x-1][ship->y-1][(int)gameState.us].x+3,positionLUT[ship->x-1][ship->y-1][(int)gameState.us].y+3,static_cast<float>(ship->type != shipDestroyer ? 22*(int)ship->type : 22*2),18},(Vector2){24/2,18/2},orientationToRotation(ship->orientation),GRAY);
      ship->render();
    }
    //DrawRectanglePro((Rectangle){positionLUT[0][0][0].x+3,positionLUT[0][0][0].y+3,static_cast<float>(22*5+size/2),18},(Vector2){24/2,18/2},orientationToRotation(orientationDown),RED);
  });
  pageManager.make("input_test",true,true,[w]()
  {
      KeyboardKey last = gameState.inputHandler.poll();
#ifdef __ANDROID__
      gameState.inputHandler.pullUpKeyboard(true);
#endif
      if (last == KEY_ENTER)
      {
        pageManager.toggle("input");
#ifdef __ANDROID__
        gameState.inputHandler.pullUpKeyboard(false);
#endif
      }
      
      DrawText(gameState.inputHandler.buff,w/2-25,5,14,GREEN);
  });
#ifdef __EMSCRIPTEN__
  if (gameState.inputHandler.iboxes.size() <= 1)
  {
    printf("iboxes where not registered");
  }
  emscripten_set_main_loop(drawEverything,0,1);
#endif


  gameState.us = 0;
  while (!WindowShouldClose())
  {
#ifdef __EMSCRIPTEN__
  drawEverything();
#else
    BeginDrawing();
			BeginMode2D(cam);
				pageManager.render();
        pageManager.renderUI();
			EndMode2D();
		EndDrawing();
#endif
    gameState.inputHandler.checkIboxes();
	}
	return 0;
}
