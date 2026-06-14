#include <windows.h>
#include "resource.h"
#include <random>
static HBITMAP titleBG = NULL;
static HBITMAP startText = NULL;
static HBITMAP optionText = NULL;
static float UIratio = 1.5f;
static int currX[3]{}, currY[3]{};

static HBITMAP hCharSelect[4] = { NULL, NULL, NULL, NULL }; // Cat, Panda, Bunny, Penguin


void pre_setUI(HINSTANCE hInstance)
{
	titleBG = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP102));
	startText = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP103));
	optionText = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP104));

	hCharSelect[0] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7));   // Cat
	hCharSelect[1] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP17));  // Panda
	hCharSelect[2] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP55));  // Bunny
	hCharSelect[3] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP65));  // Penguin
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

	for (int i = 0; i < 4; i++) {
		if (hCharSelect[i]) { DeleteObject(hCharSelect[i]); hCharSelect[i] = NULL; }
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


// 캐릭터 선택 화면 렌더링 함수
void charSelectRender(HDC mDC1, RECT win, int selectedChar)
{
	HDC mDC2 = CreateCompatibleDC(mDC1);
	BITMAP bmp;

	// 배경 (타이틀 배경 그대로 사용)
	HBITMAP oldBit = (HBITMAP)SelectObject(mDC2, titleBG);
	GetObject(titleBG, sizeof(bmp), &bmp);
	StretchBlt(mDC1, 0, -10, win.right, win.bottom,
		mDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	SetBkMode(mDC1, TRANSPARENT);
	HFONT hFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"나눔 명조");
	HFONT oldFont = (HFONT)SelectObject(mDC1, hFont);
	SetTextColor(mDC1, RGB(255, 255, 255));
	TCHAR title[] = L"캐릭터 선택 : ENTER";
	TextOut(mDC1, win.right / 2 - 120, win.bottom / 4, title, lstrlen(title));
	SelectObject(mDC1, oldFont);
	DeleteObject(hFont);

	LPCTSTR charNames[4] = { L"Cat", L"Panda", L"Bunny", L"Penguin" };

	// 4마리 배치 (화면을 4등분해서 가로로 나란히)
	int charSize = 120; // 기본 크기
	int selectedSize = 180; // 선택된 캐릭터 크기
	int totalWidth = win.right;
	int slotWidth = totalWidth / 4; 

	for (int i = 0; i < 4; i++) {
		GetObject(hCharSelect[i], sizeof(bmp), &bmp);
		SelectObject(mDC2, hCharSelect[i]);

		int drawSize = (i == selectedChar) ? selectedSize : charSize;

		int slotCenterX = slotWidth * i + slotWidth / 2;
		int drawX = slotCenterX - drawSize / 2;
		int drawY = win.bottom / 2 - drawSize / 2;

		TransparentBlt(mDC1, drawX, drawY, drawSize, drawSize,
			mDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));

		if (i == selectedChar) {
			HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 50, 50));
			HPEN oldPen = (HPEN)SelectObject(mDC1, hPen);
			HBRUSH oldBrush = (HBRUSH)SelectObject(mDC1, GetStockObject(NULL_BRUSH));
			Rectangle(mDC1, drawX - 5, drawY - 5, drawX + drawSize + 5, drawY + drawSize + 5);
			SelectObject(mDC1, oldPen);
			SelectObject(mDC1, oldBrush);
			DeleteObject(hPen);
		}

		HFONT hNameFont = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
		HFONT oldNameFont = (HFONT)SelectObject(mDC1, hNameFont);
		SetTextColor(mDC1, (i == selectedChar) ? RGB(255, 50, 50) : RGB(255, 255, 255));
		TextOut(mDC1, slotCenterX - 30, drawY + drawSize + 10, charNames[i], lstrlen(charNames[i]));
		SelectObject(mDC1, oldNameFont);
		DeleteObject(hNameFont);
	}

	SelectObject(mDC2, oldBit);
	DeleteDC(mDC2);
}