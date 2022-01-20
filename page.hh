#pragma once

#include "common.hh"

class Page : public IRenderable {
  private:
    std::function<void()> draw;
  public:
    virtual ~Page();
    std::string name;
    bool active;
    int x,y;
    bool ui;
    Page(std::string, bool, bool, std::function<void()>);
    void render();
};

