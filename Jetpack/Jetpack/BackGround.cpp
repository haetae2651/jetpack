#include <windows.h>
#include "BackGround.h"
#include "resource.h"
extern HINSTANCE hInstance;
extern HWND hWnd;
extern RECT win;



BackGround::BackGround() 
{
    hBack[7];
    hMemDC = NULL;
    hWnd = NULL;

    bmp = {}; // BITMAP의 구조체
    cameraY = 0.0f;
    newcameraY = 0.0f;
    enemyY = 0.0f;
    critical_point = 0.0f;
    cameraDelta = 0.0f;
}

BackGround::~BackGround() {
    for (int i = 0; i < 7; i++) {
        DeleteObject(hBack[i]);
    }
}

// ======== 배경로드 관련 함수 ========
void BackGround::Load(HINSTANCE hInstance) 
{
    // 비트맵 바꿀 때 IDB_BITMAP1 바꿔주기, 리소스 헤더파일 다시 가져오기
    hBack[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP101)); // 도시뷰
	hBack[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP94)); // 낮
    hBack[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP95));
    hBack[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP96));
    hBack[4] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP97));
    hBack[5] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP98));
    hBack[6] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP99));

    for (int i = 0; i < 7; i++) {
        GetObject(hBack[i], sizeof(BITMAP), &Bgbmp[i]);
    }
}


void BackGround::Render(HDC mDC1, HDC mDC2, RECT win)
{
    int score = GetScore(); // *** score에 따라 바뀌는 배경 ***

    int animStart = 400;   
    int spaceStart = 800;  // 값 고칠시에 LR장애물도 같이 고쳐야함

    int index = 0;

    if (score < animStart) {
        index = 0; // 낮
    } 
    else if (score <= animStart+15) {
        index = 1;
    }
	else if (score <= spaceStart - 60) {
		index = 2;
	}
	else if (score <= spaceStart - 45) {
		index = 3;
	}
	else if (score <= spaceStart - 30) {
		index = 4;
	}
    else if (score <= spaceStart) {
        index = 5; // 밤
    }
    else {
        index = 6;
    }

    HBITMAP OldBit = (HBITMAP)SelectObject(mDC2, hBack[index]);
    StretchBlt(mDC1, 0, 0, win.right, win.bottom,
        mDC2, 0, 0, Bgbmp[index].bmWidth, Bgbmp[index].bmHeight, SRCCOPY);
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

int BackGround::GetScore() const
{
    return static_cast<int>((-cameraY + 172) / 10);
}