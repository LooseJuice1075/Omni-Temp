#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "core/Log.h"
#include "debug/Instrumentor.h"
#include "core/Omni_Utils.h"

#ifdef OM_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <WinUser.h>
#endif
