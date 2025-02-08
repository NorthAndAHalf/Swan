#pragma once

#ifdef SD_DEBUG
#include <cassert>
#define ENGINE_ASSERT(x, msg) if (!(x)) { assert(false && msg); }
#else
#define ENGINE_ASSERT(x, msg)
#endif