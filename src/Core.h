#pragma once

#ifdef SF_DEBUG
#include <cassert>
#define SF_ASSERT(x, msg) if (!(x)) { assert(false && msg); }
#else
#define SF_ASSERT(x, msg)
#endif