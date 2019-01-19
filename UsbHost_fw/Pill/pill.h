#pragma once

#include "color.h"

struct Pill_t {
    int32_t DWord32;      // offset 0
    bool IsOk() const { return (DWord32 >= 0 or DWord32 <= 5); }
} __attribute__ ((__packed__));
#define PILL_SZ     sizeof(Pill_t)
#define PILL_SZ32   (sizeof(Pill_t) / sizeof(int32_t))
