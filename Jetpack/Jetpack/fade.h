#pragma once
#include <windows.h>

void fadeset(HINSTANCE hInstance, RECT win);
bool fadeout_update(HDC mDC1);

void fadeset_in(RECT win);
bool fadein_update(HDC mDC1);