#pragma once
#include <windows.h>

class BackGround {
private :
	HBITMAP hBack[7];
	HDC hMemDC;
	HWND hWnd;
	BITMAP Bgbmp[7];

	float cameraDelta = 0.0f; // 프레임 카메라
	float cameraY = 0.0f; // 카메라의 Y 위치
	float newcameraY = 0.0f;
	float enemyY = 0.0f;  // 적의 Y 위치

	float critical_point; // 임계점 y위치

public :
	BITMAP bmp;
	float GetCameraDelta() const;

	BackGround();
	~BackGround();

	void Load(HINSTANCE hInstance);
	void Render(HDC mDC1, HDC mDC2, RECT win);

	// 카메라 관련 함수 05.18
	void Camera_Init(int winBottom);
	void Camera_Update(float playerY);
	void Render_Objects(HDC hDC);
	float GetCameraY() const;
	int GetScore() const;
};