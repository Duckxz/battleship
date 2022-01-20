#pragma once
#include "common.hh"

namespace bship {
  enum DisconnectReason {
    kicked = 0,
    banned,
    serverFull,
    noReason
  };
  // sent by server to client when another party joins
  // sent by client to server to join a game
  struct EventJoin {
    unsigned char success; // is 0 when another player joins, 1 when you succesfully joined
    unsigned char player; // if sucess is 1, then player is our number (either 0 or 1);
  };
  
  // sent by server to client to let client know the other player left
  // sent by server to current client to let it know it was kicked
  // sent by client to server to let server know it's leaving
  struct EventLeave {
    unsigned char player; // 0 when it is current client, 1 when other player
    unsigned char reason;
  };

  // sent by client to server when drag and drop mode is enabled by server
  // sent by server to client when boat placement is forced by server
  struct EventShip {
    unsigned char type;
    unsigned char x,y;
    unsigned char orientation;
  };

  // sent by server when an attack is performed by either party
  // sent by client to attack a position
  struct EventAttack {
    unsigned char player;
    unsigned char x,y;
  };
  
  struct EventHit {
    unsigned char player;
    unsigned char x,y;
  };

  struct EventMiss {
    unsigned char player;
    unsigned char x,y;
  };

  // sent by server to both clients when turns have changed
  struct EventTurn {
    unsigned char player;
  };
  
  struct EventWin {
    unsigned char player;
  };

  struct EventConnectFailed {
    DisconnectReason reason;
  };

  typedef struct EventJoin EventJoin;
  typedef struct EventLeave EventLeave;
  typedef struct EventShip EventShip;
  typedef struct EventAttack EventAttack;
  typedef struct EventHit EventHit;
  typedef struct EventMiss EventMiss;
  typedef struct EventTurn EventTurn;
  typedef struct EventWin EventWin;
  typedef struct EventConnectFailed EventConnectFailed;

  struct Event {
    unsigned char id;
    union EventData {
      EventJoin join;
      EventLeave leave;
      EventShip ship;
      EventAttack attack;
      EventHit hit;
      EventMiss miss;
      EventTurn turn;
      EventWin win;
      EventConnectFailed connectFailed;
    } data; 
  };

  typedef struct Event Event;
}
