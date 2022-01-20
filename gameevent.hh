#pragma once
#include "common.hh"

namespace bship {
  enum EventType {
    eventAttack = 0,
    eventHit,
    eventMiss,
    eventShipPlacement,
    eventTurn,
    eventPlayerJoin,
    eventPlayerLeave,
    eventPlayerWin,
    eventConnectFailed,
    eventReset,
    eventFullReset
  };

  class GameEvent {
    public:
      EventType type;
      std::unique_ptr<Event> evt;
      GameEvent(EventType, Event);
      void handle();
  };
}
