#pragma once

#include "DebugBreak.h"
#include <iostream>

#define PANIC_FMT(msg)                                                \
  {                                                                   \
    debug_break();                                                    \
    std::cerr << "app panicked at " << msg << ", " << __FILE__ << " " \
              << __LINE__ << "\n";                                    \
    std::terminate();                                                 \
  }

#define PANIC()                  \
  {                              \
    PANIC_FMT("explicit panic"); \
  }

#define BS_ASSERT(expr)                                \
  {                                                    \
    if (!(expr)) {                                     \
      PANIC_FMT("app terminated at assertion failed"); \
    }                                                  \
  }

#define BS_ASSERT_FMT(expr, msg)                          \
  {                                                    \
    if (!(expr)) {                                     \
      PANIC_FMT(msg); \
    }                                                  \
  }