#include "NSDraw.h"

using namespace plugin;

eFontAlignment alignAt = ALIGN_LEFT;

float NSDraw::ScreenX(float x) { return (x * (SCREEN_COORD_CENTER_X * 2)) / 1024.0f; }
float NSDraw::ScreenY(float y) { return (y * (SCREEN_COORD_CENTER_Y * 2)) / 768.0f; }

void NSDraw::MDrawRect(CRect pos, CRGBA color)
{
    CSprite2d::DrawRect(CRect(ScreenX(pos.left), ScreenY(pos.top), ScreenX(pos.right), ScreenY(pos.bottom)), color);
}

void NSDraw::MDrawText(char* text, float x, float y, CRGBA color)
{
    CFont::SetOrientation(alignAt);
    CFont::SetDropShadowPosition(0);
    CFont::SetBackground(false, false);
    CFont::SetScale(ScreenX(0.35f), ScreenY(0.95f));
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetProportional(true);
    CFont::SetColor(color);
    CFont::PrintString(ScreenX(x), ScreenY(y), text);
}

void NSDraw::SetOrientation(eFontAlignment orientation) {
    alignAt = orientation;
}