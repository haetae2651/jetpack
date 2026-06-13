#include <windows.h>
#include "resource.h"
#include <random>
static HBITMAP titleBG = NULL;
static HBITMAP startText = NULL;
static HBITMAP optionText = NULL;
static float UIratio = 1.5f;
static int currX[3]{}, currY[3]{};

void pre_setUI(HINSTANCE hInstance)
{
	titleBG = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP102));
	startText = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP103));
	optionText = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP104));


}
void pre_ReleaseUI()
{
	if (titleBG) {
		DeleteObject(titleBG);
		titleBG = NULL;
	}
	if (startText) {
		DeleteObject(startText);
		startText = NULL;
	}
	if (optionText) {
		DeleteObject(optionText);
		optionText = NULL;
	}
}

void staticUIRender(HDC mDC1, RECT win)
{
	BITMAP bmp;

	GetObject(titleBG, sizeof(bmp), &bmp);
	HDC mDC2 = CreateCompatibleDC(mDC1);

	HBITMAP oldBit;
	
	oldBit = (HBITMAP)SelectObject(mDC2, titleBG);

	StretchBlt(mDC1, 0,-10, win.right, win.bottom,
		mDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	SelectObject(mDC2, oldBit);
	DeleteDC(mDC2);

}

void dynamicUIRender(HDC mDC1, RECT win, HWND hWnd,int selected)
{

	














	BITMAP bmp;

	GetObject(startText, sizeof(bmp), &bmp);
	HDC mDC2 = CreateCompatibleDC(mDC1);

	HBITMAP oldBit;


	//start
	oldBit = (HBITMAP)SelectObject(mDC2, startText);

	int smallX = bmp.bmWidth * 0.3;
	int smallY = bmp.bmHeight * 0.3;

	int bigX = smallX * UIratio;
	int bigY = smallY * UIratio;

	int targetX, targetY;

	if (selected == 0)
	{
		targetX = bigX;
		targetY = bigY;
	}
	else
	{
		targetX = smallX;
		targetY = smallY;
	}

	currX[0] += static_cast<int>(((targetX - currX[0]) * 0.2));
	currY[0] += static_cast<int>(((targetY - currY[0]) * 0.2));


	int drawX = (win.right / 2) - (currX[0] / 2);
	int drawY = (win.bottom / 2) - (currY[0] / 2);

	TransparentBlt(mDC1, drawX, drawY, currX[0], currY[0],
		mDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));


	//option
	GetObject(optionText, sizeof(bmp), &bmp);
	SelectObject(mDC2, optionText);

	smallX = bmp.bmWidth * 0.2;
	smallY = bmp.bmHeight * 0.2;

	bigX = smallX * UIratio;
	bigY = smallY * UIratio;

	if (selected == 1)
	{
		targetX = bigX;
		targetY = bigY;
	}
	else
	{
		targetX = smallX;
		targetY = smallY;
	}

	currX[1] += static_cast<int>(((targetX - currX[1]) * 0.2));
	currY[1] += static_cast<int>(((targetY - currY[1]) * 0.2));


	drawX = (win.right / 2) - (currX[1] / 2);
	drawY = (win.bottom / 2) - (currY[1] / 2) + 100;

	TransparentBlt(mDC1, drawX, drawY, currX[1], currY[1],
		mDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));

	SelectObject(mDC2, oldBit);
	DeleteDC(mDC2);

}


