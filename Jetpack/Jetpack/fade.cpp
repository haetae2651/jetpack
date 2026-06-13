#include "fade.h"
#include "resource.h"

static HBITMAP black = NULL;
static float Y = 0.0f;
static RECT fadewin = {};

void fadeset(HINSTANCE hInstance, RECT win)
{
	if (black == NULL) {
		black = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP105));
	}
	fadewin = win;
	Y = (float)win.bottom;
}

bool fadeout_update(HDC mDC1)
{
	float target = 0.0f;

	Y += (target - Y) * 0.1f;

	BITMAP bmp;
	GetObject(black, sizeof(bmp), &bmp);
	HDC mDC2 = CreateCompatibleDC(mDC1);

	HBITMAP oldBit;
	oldBit = (HBITMAP)SelectObject(mDC2, black);

	StretchBlt(mDC1, 0, (int)Y, fadewin.right, fadewin.bottom,
		mDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	SelectObject(mDC2, oldBit);
	DeleteDC(mDC2);

	if (Y <= 1.0f)
	{
		Y = 0.0f;
		return true;
	}
	return false;
}

void fadeset_in(RECT win)
{
	fadewin = win;
	Y = 0.0f; 
}

bool fadein_update(HDC mDC1)
{
	float target = (float)fadewin.bottom;

	Y += (target - Y) * 0.1f;

	BITMAP bmp;
	GetObject(black, sizeof(bmp), &bmp);
	HDC mDC2 = CreateCompatibleDC(mDC1);

	HBITMAP oldBit = (HBITMAP)SelectObject(mDC2, black);

	StretchBlt(mDC1, 0, (int)Y, fadewin.right, fadewin.bottom,
		mDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	SelectObject(mDC2, oldBit);
	DeleteDC(mDC2);

	if (target - Y <= 1.0f)
	{
		return true;
	}
	return false;
}