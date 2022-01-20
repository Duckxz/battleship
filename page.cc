#include "common.hh"

Page::Page(std::string pageName, bool ui, bool active, std::function<void()> df) : draw(df), name(pageName), active(active), ui(ui) {}
Page::~Page() {}
void Page::render() {
  this->draw();
}
