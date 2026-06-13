#include "IngameUI.h"
#include "resource.h"
static HBITMAP hEscBtn[2] = { NULL, NULL };
static HFONT hFont = NULL;

static HBITMAP hFuelBar = NULL;
static HBRUSH redBrush = NULL;
static HBRUSH oldBrush = NULL;
static HBITMAP hHeart = NULL;
static HBRUSH hHollowBrush = NULL;


void setUI(HINSTANCE hInstance) {
	//ESC 버튼
	hEscBtn[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP46)); // 흰색
	hEscBtn[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP47)); // 검은색

	//폰트 (점수)
	hFont = CreateFont(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, L"나눔 명조");

	// 연료바
	//hFuelBar = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP81));
	redBrush = CreateSolidBrush(RGB(255, 0, 0));
	hHollowBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	// 생명력
	hHeart = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP82));

	
}

void ReleaseUI() {
	for (int i = 0; i < 2; i++) {
		if (hEscBtn[i]) {
			DeleteObject(hEscBtn[i]);
			hEscBtn[i] = NULL;
		}
	}
	if (hFuelBar) {
		DeleteObject(hFuelBar);
		hFuelBar = NULL;
	}
	if (hFont) {
		DeleteObject(hFont);
		hFont = NULL;
	}
	if (redBrush) {
		DeleteObject(redBrush);
		redBrush = NULL;
	}
	if (hHeart) {
		DeleteObject(hHeart);
		hHeart = NULL;
	}
}


void scoreRender(HDC mDC1, int cameraY,RECT win) {
	//카메라 y좌표 = 점수

	HFONT oldFont = (HFONT)SelectObject(mDC1, hFont);


	SetBkMode(mDC1, TRANSPARENT);
	int score = static_cast<int>((-cameraY + 172) / 10);
	TCHAR scoreText[50];
	wsprintf(scoreText, L"Score: %d", score);
	TextOut(mDC1, 600, 10, scoreText, wcslen(scoreText));


	SelectObject(mDC1, oldFont);


}

void fuelRender(HDC mDC1, float fuel,RECT win)
{
	HFONT oldFont = (HFONT)SelectObject(mDC1, hFont);
	int intfuel = static_cast<int>(fuel);

	/*
	SetBkMode(mDC1, TRANSPARENT);
	TCHAR fuelText[50];
	wsprintf(fuelText, L"Fuel: %d", intfuel);
	TextOut(mDC1, win.right - 150, 15, fuelText, wcslen(fuelText));
	SelectObject(mDC1, oldFont);
	*/

	const int sizeY = 400;
	const int sizeX = 180;
	BITMAP bmp;
	HDC hMemDC = CreateCompatibleDC(mDC1);
	HBITMAP oldBit;

	GetObject(hFuelBar, sizeof(BITMAP), &bmp);

	oldBrush = (HBRUSH)SelectObject(mDC1, redBrush);

	float targetY = 150 + sizeY * (1 - fuel / 100);
	Rectangle(mDC1, 50, sizeY + 150, 90, static_cast<int>(targetY));
	SelectObject(mDC1, oldBrush);

	oldBrush = (HBRUSH)SelectObject(mDC1, hHollowBrush);
	Rectangle(mDC1, 50, sizeY + 150, 90, 150);
	SelectObject(mDC1, oldBrush);


	
	DeleteDC(hMemDC);
}

void escRender(HDC mDC1, HINSTANCE hInstance, RECT win, bool isStop) {
	// 비트맵 로드
		int size = 40;



	BITMAP bmp;
	GetObject(hEscBtn[0], sizeof(BITMAP), &bmp);
	
	HDC hMemDC = CreateCompatibleDC(mDC1);

	HBITMAP oldBit;
	if (isStop) {
		oldBit = (HBITMAP)SelectObject(hMemDC, hEscBtn[1]); // 검은색
	}
	else {
		oldBit = (HBITMAP)SelectObject(hMemDC, hEscBtn[0]); // 흰색
	}
	
	TransparentBlt(mDC1, 10, 10, 40, 40,
		hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));

	SelectObject(hMemDC, oldBit);
	DeleteDC(hMemDC);
}

void HeartRender(HDC mDC1, int hp, RECT win) {
	BITMAP bmp;
	
	GetObject(hHeart, sizeof(BITMAP), &bmp);
	HDC hMemDC = CreateCompatibleDC(mDC1);
	HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hHeart);
	
	int sizex = bmp.bmWidth - 125;
	int sizey = bmp.bmHeight - 125;
	for (int i = 0; i < hp; i++) {
		TransparentBlt(mDC1, win.right - 20 - sizex - (i * sizex) - (10 * i), 10, sizex, sizey,
			hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));
	}
	SelectObject(hMemDC, oldBit);
	DeleteDC(hMemDC);
}	


