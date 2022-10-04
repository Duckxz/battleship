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

#pragma once
#include "common.hh"

enum IboxType {
  iboxGeneric = 0,
  iboxUIButton,
  iboxBoat,
  iboxCell
};

struct ibox {
    enum IboxType type;
    Rectangle spot;
    std::function<void()> click;
    ibox(IboxType,Rectangle,std::function<void()>);
};

typedef struct ibox ibox;

class InputHandler {
  private:
#ifdef __EMSCRIPTEN__
    std::chrono::time_point<std::chrono::high_resolution_clock> lastClick;
#endif
    int _buffIdx;
#ifdef __ANDROID__
    bool _isSoftKeyboardUp;
#endif
  public:
    std::vector<ibox*> iboxes;
    char* buff;
    bool wipeOnDelete;
    int buffMax;
    InputHandler(int);
    ~InputHandler();
    KeyboardKey poll();
    bool pollMouse(MouseButton,int*,int*);
    void checkIboxes();
#ifdef __ANDROID__
    void pullUpKeyboard(bool should);
#endif
};
