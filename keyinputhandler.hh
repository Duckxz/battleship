#pragma once
#include "common.hh"

namespace bship {
  class KeyInputHandler : public InputHandler {
    private:
      std::vector<char> buff;
    public:
      KeyInputHandler(std::function(bool()))
      void Poll();
      char Fetch();
      std::string FetchLine();
  };
}
