#pragma once
#include <windows.h>
void staticUIRender(HDC mDC1, RECT win);
void dynamicUIRender(HDC mDC1, RECT win,HWND hWnd,int selected);

void pre_setUI(HINSTANCE hInstance);
void pre_ReleaseUI();
