#pragma once
#ifdef __ANDROID__
#include <android_native_app_glue.h>
#include <android/log.h>
#include <jni.h>
#include "raylib.h" // for some reason android doesn't search in host sysroot so we just put a local copy in the source
#else
#include <raylib.h>
#endif
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <iostream>
#include <dirent.h>
#include <string>
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>
#include <vector>
#include <unordered_map>

#define NUMCELLS 10

#include "irenderable.hh"
#include "animation.hh"
#include "ship.hh"
#include "sprite.hh"
#include "page.hh"
#include "pagemanager.hh"
//#include "ship.hh"
#include "shipmanager.hh"
#include "inputhandler.hh"
#include "state.hh"
#include "battleship.hh"
#include "event.hh"
#include "gameevent.hh"
