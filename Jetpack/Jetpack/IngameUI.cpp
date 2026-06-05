#include "IngameUI.h"
#include "resource.h"

void scoreRender(HDC mDC1, int cameraY,RECT win) {
	//카메라 y좌표 = 점수

	HFONT hFont = CreateFont(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, L"나눔 명조");
	HFONT oldFont = (HFONT)SelectObject(mDC1, hFont);


	SetBkMode(mDC1, TRANSPARENT);
	int score = static_cast<int>((-cameraY + 172) / 10);
	TCHAR scoreText[50];
	wsprintf(scoreText, L"Score: %d", score);
	TextOut(mDC1, 600, 10, scoreText, wcslen(scoreText));


	SelectObject(mDC1, oldFont);
	DeleteObject(hFont);


}

void fuelRender(HDC mDC1, float fuel,RECT win)
{
	HFONT hFont = CreateFont(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, L"나눔 명조");
	HFONT oldFont = (HFONT)SelectObject(mDC1, hFont);
	int intfuel = static_cast<int>(fuel);

	SetBkMode(mDC1, TRANSPARENT);
	TCHAR fuelText[50];
	wsprintf(fuelText, L"Fuel: %d", intfuel);
	TextOut(mDC1, win.right - 150, 15, fuelText, wcslen(fuelText));


	SelectObject(mDC1, oldFont);
	DeleteObject(hFont);
}

void escRender(HDC mDC1, HINSTANCE hInstance, RECT win, bool isStop) {
	// 비트맵 로드
	HBITMAP hEscBtn[2];
	int size = 40;

	hEscBtn[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP46)); // 흰색
	hEscBtn[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP47)); // 검은색

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
	DeleteObject(hEscBtn[0]);
	DeleteObject(hEscBtn[1]);
}