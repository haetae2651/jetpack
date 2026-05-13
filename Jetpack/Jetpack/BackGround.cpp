#include <windows.h>
#include "BackGround.h"
#include "resource.h"
extern HINSTANCE hInstance;
extern HWND hWnd;
extern RECT win;

BackGround::BackGround() 
{
    hBitmap = NULL;
    hMemDC = NULL;
    hWnd = NULL;
    bmp = {}; // BITMAP�� ����ü��
}

BackGround::~BackGround() {}

void BackGround::Load(HINSTANCE hInstance) 
{
	hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));// �̰� ���ҽ� ��Ʈ�� �߰��ϰ� �ּ� Ǯ�� ��
    GetObject(hBitmap, sizeof(bmp), &bmp);
}

void BackGround::Render(HDC hDC, RECT win) 
{
    hMemDC = CreateCompatibleDC(hDC); //--- �޸� DC �����
    (HBITMAP)SelectObject(hMemDC, hBitmap); //--- ��Ʈ�ʰ� �޸� DC �����ϱ�
    StretchBlt(hDC, 0, 0, win.right, win.bottom, 
        hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); //--- ������ ũ�⿡ �±� ��Ʈ�� �׷��ֱ�
    DeleteDC(hMemDC);
}