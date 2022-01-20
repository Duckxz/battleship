#pragma once
#include "common.hh"

class PageManager : public IRenderable {
  public:
    std::unordered_map<std::string,std::unique_ptr<Page>> pages;
    void render();
    void renderUI();
    void render(std::string);
    void make(std::string, bool, bool, std::function<void()>);
    bool toggle(std::string);
    bool toggle(std::string, bool);
};
