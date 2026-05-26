#include "IngameUI.h"

void scoreRender(HDC mDC1, int cameraY,RECT win) {
	//카메라 y좌표 = 점수

	HFONT hFont = CreateFont(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, L"나눔 명조");
	HFONT oldFont = (HFONT)SelectObject(mDC1, hFont);


	SetBkMode(mDC1, TRANSPARENT);
	int score = static_cast<int>((-cameraY + 172) / 10);
	TCHAR scoreText[50];
	wsprintf(scoreText, L"Score: %d", score);
	TextOut(mDC1, 10, 10, scoreText, wcslen(scoreText));


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