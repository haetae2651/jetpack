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
    cameraY = 0.0f;
    newcameraY = 0.0f;
    enemyY = 0.0f;
    critical_point = 0.0f;
    cameraDelta = 0.0f;
}

BackGround::~BackGround() {}

// ======== 배경로드 관련 함수 ========
void BackGround::Load(HINSTANCE hInstance) 
{
    // 비트맵 바꿀 때 IDB_BITMAP1 바꿔주기, 리소스 헤더파일 다시 가져오기
	hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
    GetObject(hBitmap, sizeof(bmp), &bmp);
}

void BackGround::Render(HDC mDC1,HDC mDC2, RECT win) 
{
	HBITMAP OldBit = (HBITMAP)SelectObject(mDC2, hBitmap); // 비트맵과 메모리 DC 연결
    StretchBlt(mDC1, 0, 0, win.right, win.bottom, 
		mDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); // 화면의 크기에 맞게 비트맵을 그리기
    SelectObject(mDC2, OldBit);
}

// ======== 카메라 관련 함수 ========
void BackGround::Camera_Init(int winBottom)
{
    cameraY = winBottom / 2; // 카메라 초기값
	critical_point = winBottom * 0.6; // 임계점 초기화
}

void BackGround::Camera_Update(float playerY) 
{
	float prevCameraY = cameraY; // 이전 프레임의 카메라 Y 위치 저장
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
    cameraDelta = cameraY - prevCameraY;
}

float BackGround::GetCameraDelta() const // ← 추가
{
    return cameraDelta;
}

void BackGround::Render_Objects(HDC hDC) 
{

}

float BackGround::GetCameraY() const  // cameraY변수 가져오기
{
    return cameraY;
}