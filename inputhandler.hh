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
