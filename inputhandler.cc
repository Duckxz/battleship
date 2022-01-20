#include "common.hh"

InputHandler::InputHandler(int buffMax)
{
#ifdef __ANDROID__
  this->_isSoftKeyboardUp = false;
#endif
  this->wipeOnDelete = true;
  this->buffMax = buffMax; 
  this->buff = (char*)malloc(buffMax);
  memset(this->buff,0,buffMax);
}

InputHandler::~InputHandler()
{
  free(this->buff);
  for (ibox* ibox : this->iboxes)
  {
    delete ibox;
  }
}

bool InputHandler::pollMouse(MouseButton button, int* resX, int* resY)
{
  int cursorX,cursorY;
  *resX = 0;
  *resY = 0;
#ifdef __ANDROID__
  cursorX = GetTouchX();
  cursorY = GetTouchY();
#else
  cursorX = GetMouseX();
  cursorY = GetMouseY();
#endif

#ifdef __EMSCRIPTEN__ // since web can only check wether a mouse button is down it will continuously fire as our FPS is higher than it takes until a human releases the mouse button
  float msSinceLastClick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->lastClick).count();
  if (msSinceLastClick <= 175.0f)
  {
    return false;
  }
#endif

  if (
#ifdef __EMSCRIPTEN__ // very ugly but it's the only way
      IsMouseButtonDown(button)
#elif defined(__ANDROID__)
      GetGestureDetected() == GESTURE_TAP
#else
      IsMouseButtonPressed(button)
#endif
    )
    {
#ifdef __EMSCRIPTEN__
      this->lastClick = std::chrono::high_resolution_clock::now();
#endif
      printf("click at %d , %d\n",cursorX,cursorY);
      *resX = cursorX;
      *resY = cursorY;
      return true;
    }
    else
    {
      return false;
    }
}

KeyboardKey InputHandler::poll()
{
  KeyboardKey lastKey = (KeyboardKey)GetKeyPressed();
  char lastChar = GetCharPressed();
#ifdef __ANDROID__
  __android_log_print(ANDROID_LOG_VERBOSE,"InputHandler:Poll","lastKey: %d",lastKey);
  __android_log_print(ANDROID_LOG_VERBOSE,"InputHandler::Poll","lastChar: %c",lastChar);
#endif
  if (this->_buffIdx < this->buffMax && lastKey != KEY_DELETE && lastKey != KEY_NULL && lastChar != 0)
  {
    this->buff[this->_buffIdx] = lastChar;
    this->_buffIdx = (this->_buffIdx+1 < this->buffMax) ? this->_buffIdx + 1 : this->buffMax-1;
  }
  else if (this->_buffIdx < this->buffMax && this->_buffIdx >= 0 && lastKey == KEY_BACKSPACE)
  {
    this->buff[this->_buffIdx] = '\0';
    this->_buffIdx = this->_buffIdx - 1 >= 0 ? this->_buffIdx - 1 : 0;
  }
  else if (this->wipeOnDelete && lastKey == KEY_DELETE)
  {
    memset(this->buff,0,this->buffMax);
    this->_buffIdx = 0;
  }
  return lastKey;
}

ibox::ibox(IboxType type, Rectangle r, std::function<void()> lam) : type(type), spot(r), click(lam)
{}

void InputHandler::checkIboxes()
{
  for (ibox* inputBox: this->iboxes)
  //for (std::vector<ibox*>::iterator inputBox = this->iboxes.begin(); inputBox != this->iboxes.end(); ++inputBox)
  {
    int cursorX, cursorY;
    bool mouseClicked = gameState.inputHandler.pollMouse(MOUSE_BUTTON_LEFT,&cursorX,&cursorY);
    if (mouseClicked)
    {
      printf("click! checking wether %d,%d intersects with rectangle at %f, %f with %f,%f as dimensions\n",cursorX,cursorY,inputBox->spot.x,inputBox->spot.y,inputBox->spot.width,inputBox->spot.height);
    }
    if (mouseClicked && CheckCollisionPointRec((Vector2){(float)cursorX,(float)cursorY},inputBox->spot))
    {
#ifdef __EMSCRIPTEN__
      this->lastClick = std::chrono::high_resolution_clock::now();
#endif
      inputBox->click();
    }
  }
}


// i tested this extensively and it does *not* work, you can't use the soft keyboard in a NativeActivity
#ifdef __ANDROID__
void InputHandler::pullUpKeyboard(bool should) {
  /*if (!should && this->isSoftKeyboardUp)
    {
    __android_log_print(ANDROID_LOG_VERBOSE,"[InputHandler::pullUpKeyboard]","hiding android soft keyboard");
    ANativeActivity_hideSoftInput(gameState.app->activity,ANATIVEACTIVITY_HIDE_SOFT_INPUT_IMPLICIT_ONLY);
    this->_isSoftKeyboardUp = false;
  }
  else if (should && !this->_isSoftKeyboardUp)
  {
    __android_log_print(ANDROID_LOG_VERBOSE,"[InputHandler::pullUpKeyboard]","showing android soft keyboard");
    ANativeActivity_showSoftInput(gameState.app->activity,ANATIVEACTIVITY_SHOW_SOFT_INPUT_FORCED);
    this->)isSoftKeyboardUp = true;
  }
  else if ((should && this->_isSoftKeyboardUp) || (!should && !this->_isSoftKeyboardUp))
  {
    return; // do nothing when the keyboard is already hidden/shown
  }*/
  // Attaches the current thread to the JVM.
  jint lResult;
  jint lFlags = 0;

  JavaVM* lJavaVM = gameState.app->activity->vm;
  JNIEnv* lJNIEnv;

  JavaVMAttachArgs lJavaVMAttachArgs;
  lJavaVMAttachArgs.version = JNI_VERSION_1_6;
  lJavaVMAttachArgs.name = "NativeThread";
  lJavaVMAttachArgs.group = NULL;

  bool attached = false;
  switch (lJavaVM->GetEnv((void**)&lJNIEnv,JNI_VERSION_1_6))
  {
    case JNI_OK:
      break;
    case JNI_EDETACHED:
      if (lJavaVM->AttachCurrentThread(&lJNIEnv,&lJavaVMAttachArgs)!=0)
      {
        throw std::runtime_error("could not attach current thread to JVM");
      }
      attached = true;
      break;
    case JNI_EVERSION:
      throw std::runtime_error("Invalid JVM java version");
      break;
  }

  // Retrieves NativeActivity.
  jobject lNativeActivity = gameState.app->activity->clazz;
  jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

  // Retrieves Context.INPUT_METHOD_SERVICE.
  jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
  jfieldID FieldINPUT_METHOD_SERVICE =lJNIEnv->GetStaticFieldID(ClassContext,"INPUT_METHOD_SERVICE", "Ljava/lang/String;");
  jobject INPUT_METHOD_SERVICE = lJNIEnv->GetStaticObjectField(ClassContext,FieldINPUT_METHOD_SERVICE);

  // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
  jclass ClassInputMethodManager = lJNIEnv->FindClass("android/view/inputmethod/InputMethodManager");
  jmethodID MethodGetSystemService = lJNIEnv->GetMethodID(ClassNativeActivity, "getSystemService","(Ljava/lang/String;)Ljava/lang/Object;");
  jobject lInputMethodManager = lJNIEnv->CallObjectMethod(lNativeActivity, MethodGetSystemService,INPUT_METHOD_SERVICE);

  // Runs getWindow().getDecorView().
  jmethodID MethodGetWindow = lJNIEnv->GetMethodID(ClassNativeActivity, "getWindow","()Landroid/view/Window;");
  jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity,MethodGetWindow);
  jclass ClassWindow = lJNIEnv->FindClass("android/view/Window");
  jmethodID MethodGetDecorView = lJNIEnv->GetMethodID(ClassWindow, "getDecorView", "()Landroid/view/View;");
  jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow,MethodGetDecorView);

  if (should /*&& !this->_isSoftKeyboardUp*/) // spamming it keeps it up for some reason?
  {
    // Runs lInputMethodManager.showSoftInput(...).
    jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID(ClassInputMethodManager, "showSoftInput","(Landroid/view/View;I)Z");
    jboolean lResult = lJNIEnv->CallBooleanMethod(lInputMethodManager, MethodShowSoftInput,lDecorView, lFlags);
    this->_isSoftKeyboardUp = true;
    __android_log_print(ANDROID_LOG_VERBOSE,"InputHandler::pullupKeyboard","showing soft keyboard");
  }
  else if (!should && this->_isSoftKeyboardUp)
  {
    // Runs lWindow.getViewToken()
    jclass ClassView = lJNIEnv->FindClass("android/view/View");
    jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID(ClassView, "getWindowToken", "()Landroid/os/IBinder;");
    jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView,MethodGetWindowToken);

    // lInputMethodManager.hideSoftInput(...).
    jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID(
    ClassInputMethodManager, "hideSoftInputFromWindow","(Landroid/os/IBinder;I)Z");
    jboolean lRes = lJNIEnv->CallBooleanMethod(lInputMethodManager, MethodHideSoftInput,lBinder, lFlags);
    this->_isSoftKeyboardUp = false;
    __android_log_print(ANDROID_LOG_VERBOSE,"InputHandler::pullupKeyboard","hiding soft keyboard");
  }

  // Finished with the JVM.
  lJavaVM->DetachCurrentThread(); 
}
#endif
