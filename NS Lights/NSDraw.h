#pragma once

#include "plugin.h"

#include "CSprite2d.h"
#include "CFont.h"

class NSDraw {
public:
    static float ScreenX(float x);
    static float ScreenY(float y);

    static void MDrawRect(CRect pos, CRGBA color);
    static void MDrawText(char* text, float x, float y, CRGBA color);
    static void SetOrientation(eFontAlignment orientation);
};