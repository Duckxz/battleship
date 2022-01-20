#include "common.hh"

void PageManager::render() {
 for(const auto &p : this->pages) {
  if(p.second->active && !p.second->ui) {
    p.second->render();
  }
 } 
}

void PageManager::render(std::string page_name) {
  try {
    this->pages.at(page_name)->render();
  } catch(std::out_of_range& ex) {
    return; // page doesn't exist, let's not render it i guess
  } 
}

void PageManager::renderUI() {
  for(const auto &p: this->pages) {
    if(p.second->active && p.second->ui) {
      p.second->render();
    }
  }
}

void PageManager::make(std::string name, bool ui, bool active, std::function<void()> df) {
  this->pages.emplace(name,std::make_unique<Page>(name,ui,active,df));
}

bool PageManager::toggle(std::string name) {
  for(auto &p : this->pages) {
    if(p.second->name == name) {
      p.second->active = (p.second->active == true) ? false : true;
      return (p.second->active);
    }
  }
  return false;
}

bool PageManager::toggle(std::string name, bool activate) {
  for(auto &p : this->pages) {
    if(p.second->name == name) {
      p.second->active = activate;
      return (p.second->active);
    }
  }
  return false;
}
