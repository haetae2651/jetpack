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
	HDC mDC;
	HBITMAP hBitmap;
	static RECT win;


	//플레이어 관련 변수
	static int playerX, playerY; // 플레이어 위치 변수
	static int maxSpeed = 10;
	static Player player;



	//임시 테스트용 장애물
	static Obstacles tempObs{ 0,{1000,-10},100 };
	static int obsX, obsY;
	static int obsSize = tempObs.getSize();
	


	//BackGround 변수
	static BackGround bg;
	static float cameraY;


	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &win);		
		SetTimer(hWnd, 1, 1, NULL);		
		player.setPos({ win.right / 2, win.bottom / 2 });	

		bg.Load(g_hInst);
		bg.Camera_Init(win.bottom);
		cameraY = bg.GetCameraY();
		break;


	case WM_CHAR:
		switch (wParam) {

		case 'q':
				PostQuitMessage(0);
				break;
		}

		break;



	case WM_KEYDOWN:





		break;
	case WM_TIMER:

		//플레이어 이동처리
		player.decel();				
		player.update();			
		player.move(wParam,maxSpeed);



		// 카메라 임계점 5.18
		bg.Camera_Update(player.getPos().y);
		cameraY = bg.GetCameraY();

		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT: {

		hDC = BeginPaint(hWnd, &ps);

		
		mDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, win.right, win.bottom);
		SelectObject(mDC, hBitmap);

		bg.Render(mDC, win);



		playerX = player.getPos().x;
		playerY = player.getPos().y;

		
		

		// ***********유하영이 " - cameraY " 추가했다***********
		Rectangle(mDC, playerX - 10, playerY - cameraY - 10, playerX + 10, playerY - cameraY + 10);   //플레이어 그리기(임시 사각형)


		//----------------------임시 테스트용 장애물 그리기
		obsX = tempObs.getPos().x;
		obsY = tempObs.getPos().y;
		Rectangle(mDC, obsX - obsSize / 2, obsY - cameraY - obsSize / 2, obsX + obsSize / 2, obsY - cameraY + obsSize / 2);   //장애물 그리기(임시 사각형)
		TCHAR str[10];
		wsprintf(str, L"장애물", playerY);
		TextOut(mDC, obsX - 50, obsY - cameraY, str, lstrlen(str));
		//----------------------






		BitBlt(hDC, 0, 0, win.right, win.bottom, mDC, 0, 0, SRCCOPY);


		DeleteObject(hBitmap);
		DeleteDC(mDC);
		EndPaint(hWnd, &ps);
		break;
	}



	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}