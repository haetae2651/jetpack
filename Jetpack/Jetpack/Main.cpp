#include <windows.h>
#include <tchar.h>
#include "Player.h"
#include "BackGround.h"
#include "Obstacles.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"My Window Class";
LPCTSTR lpszWindowName = L"Jetpack";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindowW(
		lpszClass,
		lpszWindowName,
		WS_POPUP | WS_VISIBLE, 
		0, 0,                  
		GetSystemMetrics(SM_CXSCREEN), 
		GetSystemMetrics(SM_CYSCREEN),
		nullptr, nullptr, hInstance, nullptr
	);	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;


}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HDC mDC1, mDC2;
	static HBITMAP hBitmap;
	static RECT win;
	HBITMAP OldBit[1]; // Old 비트맵 저장용

	static int bWidth, bHeight;

	// 플레이어 관련 변수
	static int playerX, playerY;
	static int maxSpeed = 10;
	static Player player;

	// 임시 테스트용 장애물
	static Obstacles tempObs{ 0, {1000, -10}, 100 };
	static int obsX, obsY;
	static int obsSize = tempObs.getSize();


	static BackGround bg;
	static float cameraY;

	switch (uMsg) {
	case WM_CREATE:
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &win);
		SetTimer(hWnd, 1, 1, NULL);
		player.setPos({ win.right / 2, win.bottom / 2 });

		hBitmap = CreateCompatibleBitmap(hDC, win.right, win.bottom);

		bg.Load(g_hInst);
		bg.Camera_Init(win.bottom);
		cameraY = bg.GetCameraY();

		bWidth = bg.bmp.bmWidth;
		bHeight = bg.bmp.bmHeight;

		ReleaseDC(hWnd, hDC); // GetDC를 했으므로 반드시 ReleaseDC 해주어야 합니다.
		break;

	case WM_CHAR:
		switch (wParam) {
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_TIMER:
	{
		hDC = GetDC(hWnd);
		mDC1 = CreateCompatibleDC(hDC);
		mDC2 = CreateCompatibleDC(mDC1);

		// 1. 메모리 누수 방지를 위해 이전 비트맵을 저장해둡니다.
		HBITMAP oldMDC1Bit = (HBITMAP)SelectObject(mDC1, hBitmap);

		PatBlt(mDC1, 0, 0, win.right, win.bottom, WHITENESS);
		bg.Render(mDC1, mDC2, win); // mDC1으로 쏨

		playerX = player.getPos().x;
		playerY = player.getPos().y;

		Rectangle(mDC1, playerX - 10, playerY - cameraY - 10, playerX + 10, playerY - cameraY + 10); // 플레이어 그리기

		//----------------------임시 테스트용 장애물 그리기
		obsX = tempObs.getPos().x;
		obsY = tempObs.getPos().y;
		Rectangle(mDC1, obsX - obsSize / 2, obsY - cameraY - obsSize / 2, obsX + obsSize / 2, obsY - cameraY + obsSize / 2);
		TCHAR str[20];
		wsprintf(str, L"플레이어 Y: %d", playerY); 
		TextOut(mDC1, obsX - 50, obsY - cameraY, str, lstrlen(str));
		//----------------------

		bg.Camera_Update(player.getPos().y);
		cameraY = bg.GetCameraY();

	
		player.decel();
		player.update();
		player.move(wParam, maxSpeed); 

		// 3. DC 원상복구 및 해제
		SelectObject(mDC1, oldMDC1Bit);
		DeleteDC(mDC2);
		DeleteDC(mDC1);
		ReleaseDC(hWnd, hDC);

		// WM_PAINT 호출 (백그라운드 지우기를 막기 위해 FALSE 유지)
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_PAINT: {
		hDC = BeginPaint(hWnd, &ps);
		mDC1 = CreateCompatibleDC(hDC);

		OldBit[0] = (HBITMAP)SelectObject(mDC1, hBitmap);

		BitBlt(hDC, 0, 0, win.right, win.bottom, mDC1, 0, 0, SRCCOPY);

		SelectObject(mDC1, OldBit[0]);

		DeleteDC(mDC1);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
		KillTimer(hWnd, 1);       // 타이머 해제
		DeleteObject(hBitmap);    // 프로그램이 끝날 때 도화지(hBitmap)를 파괴합니다.
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}