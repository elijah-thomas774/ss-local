#ifndef M_COLOR_H
#define M_COLOR_H

#include "nw4r/ut/ut_Color.h"

struct mColor : public nw4r::ut::Color {
    mColor() : nw4r::ut::Color() {}
    mColor(int red, int green, int blue, int alpha) : nw4r::ut::Color(red, green, blue, alpha) {}
    mColor(int hex) : nw4r::ut::Color(hex) {}
};

#endif
