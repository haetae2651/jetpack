#include <windows.h>
#include <tchar.h>
#include "Player.h"
#include "BackGround.h"

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
		GetSystemMetrics(SM_CXSCREEN), // 전체 가로 길이
		GetSystemMetrics(SM_CYSCREEN), // 전체 세로 길이
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

	static int playerX, playerY; // 플레이어 위치 변수

	static int maxSpeed = 10;
	static Player player;
	
	static RECT win;

	// ------------------------ BackGround 변수
	static BackGround bg;


	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &win);		// win: 창 클라이언트의 크기
		SetTimer(hWnd, 1, 1, NULL);		// 타이머 설정 (1ms)
		player.setPos({ win.right / 2, win.bottom / 2 });	// 플레이어 초기 위치 설정 (창 중앙)

		bg.Load(g_hInst); // 배경 비트맵 불러오기
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
		player.decel();				//감속은 항상 적용
		player.update();			//위치 업데이트
		player.move(wParam,maxSpeed);		//좌우 이동


		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT: {

		hDC = BeginPaint(hWnd, &ps);

		
		
		//더블 버퍼링을 위한 메모리 DC와 비트맵 생성
		mDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, win.right, win.bottom);
		SelectObject(mDC, hBitmap);

		bg.Render(mDC, win); // 5월12일 배경 비트맵 그리기 - mDC사용

		// FillRect(mDC, &win, (HBRUSH)GetStockObject(WHITE_BRUSH));		//mDC 배경 흰색으로 채우기
		// (내가 하늘배경으로 바꿈)

		playerX = player.getPos().x;
		playerY = player.getPos().y;




		Rectangle(mDC, playerX - 10, playerY - 10, playerX + 10, playerY	 + 10);   //플레이어 그리기(임시 사각형)



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