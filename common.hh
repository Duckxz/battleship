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
