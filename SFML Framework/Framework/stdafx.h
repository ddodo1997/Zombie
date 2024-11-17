#pragma once

#include <random>
#include <list>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <fstream>
#include <locale>
#include <codecvt>
#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Defines.h"
#include "Utils.h"
#include "Singleton.h"
#include "Framework.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "TextGo.h"
#include "SpriteGo.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "ObjectPool.h"
#include "DebugBox.h"
#include "SoundMgr.h"
#include "rapidcsv.h"
#include "DataTable.h"
#include "StringTable.h"
#include "DataTableMgr.h"
#include "Variables.h"
#include "AnimationClip.h"
#include "Animator.h"
#include "json.hpp"
using json = nlohmann::json;
#include "SaveLoadMgr.h"