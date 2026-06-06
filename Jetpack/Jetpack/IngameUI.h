#pragma once
#include <windows.h>
void scoreRender(HDC mDC1, int cameraY, RECT win);
void fuelRender(HDC mDC1, float fuel, RECT win);
void escRender(HDC mDC1, HINSTANCE hInstance, RECT win, bool isStop);

void setUI(HINSTANCE hInstance);
void ReleaseUI();