#include <windows.h>
#include "resource.h"
#include "BackGround.h"

extern HINSTANCE hInstance;
extern HWND hWnd;
extern RECT win;

BackGround::BackGround() 
{
    hBitmap = NULL;
    hMemDC = NULL;
    hWnd = NULL;
    bmp = {}; // BITMAP은 구조체임
}

BackGround::~BackGround() {}

void BackGround::Load(HINSTANCE hInstance) 
{
    hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
    GetObject(hBitmap, sizeof(bmp), &bmp);
}

void BackGround::Render(HDC hDC, RECT win) 
{
    hMemDC = CreateCompatibleDC(hDC); //--- 메모리 DC 만들기
    (HBITMAP)SelectObject(hMemDC, hBitmap); //--- 비트맵과 메모리 DC 연결하기
    StretchBlt(hDC, 0, 0, win.right, win.bottom, 
        hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); //--- 윈도우 크기에 맞기 비트맵 그려주기
    DeleteDC(hMemDC);
}