#pragma once
#include "common.hh"

class IRenderable {
  public:
    virtual void render() = 0;
};
