#pragma once

#include <cstring>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <memory>
#include <vector>
#include <limits>
#include <stack>
#include <algorithm>
#include <sstream>
#include <utility>
#include <iterator>
#include <functional>

#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <type_traits>

#include <cmath>

#if defined(_WIN32)
#define STRICT              1
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <sdkddkver.h>
#include <debugapi.h>
#undef min
#undef max
#endif // defined(_WIN32)

#define __STR_CAT(A, B) A##B
#define _STR_CAT(A, B)  __STR_CAT(A, B)
