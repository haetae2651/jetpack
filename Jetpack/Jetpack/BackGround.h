#pragma once
#include <windows.h>

class BackGround {
private :
	HBITMAP hBitmap;
	HDC hMemDC;
	HWND hWnd;
	BITMAP bmp;
public :
	BackGround();
	~BackGround();

	void Load(HINSTANCE hInstance);
	void Render(HDC hDC, RECT win);
};