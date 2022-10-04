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
