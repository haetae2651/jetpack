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
    bmp = {}; // BITMAP의 구조체
}

BackGround::~BackGround() {}

// ======== 배경로드 관련 함수 ========
void BackGround::Load(HINSTANCE hInstance) 
{
    // 비트맵 바꿀 때 IDB_BITMAP1 바꿔주기, 리소스 헤더파일 다시 가져오기
	hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
    GetObject(hBitmap, sizeof(bmp), &bmp);
}

void BackGround::Render(HDC hDC, RECT win) 
{
    hMemDC = CreateCompatibleDC(hDC); // 메모리 DC 생성
	(HBITMAP)SelectObject(hMemDC, hBitmap); // 비트맵과 메모리 DC 연결
    StretchBlt(hDC, 0, 0, win.right, win.bottom, 
		hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); // 화면의 크기에 맞게 비트맵을 그리기

    DeleteDC(hMemDC);
}

// ======== 카메라 관련 함수 ========
void BackGround::Camera_Init(int winBottom)
{
    cameraY = winBottom / 2; // 카메라 초기값
	critical_point = winBottom * 0.3; // 임계점 초기화
}

void BackGround::Camera_Update(float playerY) 
{
	float screenY = playerY - cameraY;

    if (screenY < critical_point) {
		newcameraY = playerY - critical_point; // 임계점 도달 후, 다시 내려올 때 카메라 newcameraY
        if (newcameraY < cameraY) {
            cameraY = newcameraY;
        }
    }

    if (critical_point == playerY) {
        cameraY = newcameraY;
    }
}

void BackGround::Render_Objects(HDC hDC) 
{

}

float BackGround::GetCameraY() const  // cameraY변수 가져오기
{
    return cameraY;
}