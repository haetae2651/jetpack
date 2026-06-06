#include <windows.h>
#include <tchar.h>
#include <random>

#include "Player.h"
#include "BackGround.h"


#include "Obstacles.h"
#include "ObstacleManager.h"

#include "OBS_Random.h"
#include "OBS_Path.h"
#include "OBS_LeftRight.h"

#include "IngameUI.h"

using namespace std;

default_random_engine dre{ random_device{}() };

uniform_int_distribution<int> randtype(0, 3);


int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"My Window Class";
LPCTSTR lpszWindowName = L"Jetpack";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{


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
		screenWidth, 
		screenHeight,
		nullptr, nullptr, hInstance, nullptr
	);	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;


}

void Game_Stop(HWND hWnd, bool& isStop) {
	isStop = !isStop;
	if (isStop) {
		KillTimer(hWnd, 1);
	}
	else {
		SetTimer(hWnd, 1, 1, NULL);
	}
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
	static bool playerseen = true;
	static int seencnt = 0;

	// 임시 테스트용 장애물
	static Obstacles tempObs{ 0, {1000, -10}, 100 };
	static int obsX, obsY;
	static int obsSize = tempObs.getSize();

	static ObstacleManager obsManager; // 5.27
	static BackGround bg;
	static float cameraY;

	//6.2 충돌 히트박스
	static RECT playerRect;

	//UI 관련 변수
	static bool IngameUI_Render = false;

	//6.2 게임설정
	static bool isStop = false; // 게임 멈추기

	switch (uMsg) {
	case WM_CREATE:

		IngameUI_Render = true;



		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &win);

		SetTimer(hWnd, 1, 1, NULL);

		player.setPos({ win.right / 2, win.bottom / 2 });

		hBitmap = CreateCompatibleBitmap(hDC, win.right, win.bottom);

		//배경 초기화
		bg.Load(g_hInst);
		bg.Camera_Init(win.bottom);

		// 카메라 초기화
		cameraY = bg.GetCameraY();
		bWidth = bg.bmp.bmWidth;
		bHeight = bg.bmp.bmHeight;

		// 플레이어 초기화
		player.setImage(g_hInst);

		// 5.27 랜덤장애물 초기화
		obsManager.setWin(win);
		for (int i = 0; i < 5; i++) {
			POINT p = { rand() % 600 + 100, -(i * 200) };
			obsManager.Add_Obstacle(new OBS_Random(p, g_hInst));

		
		}

		obsManager.Add_Obstacle(new OBS_LeftRight({ 0+10, -200 }, g_hInst,5));

		obsManager.Add_Obstacle(new OBS_Path({ 0, -1000 }, g_hInst, win.right));




		ReleaseDC(hWnd, hDC);
		break;


	case WM_LBUTTONDOWN: {
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		// 왼쪽 상단 버튼 영역 (버튼 크기에 맞게 조절)
		if (mouseX >= 10 && mouseX <= 10 + 50 && mouseY >= 10 && mouseY <= 10 + 50) {
			Game_Stop(hWnd, isStop);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	}


	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
			player.setType(1);
			break;
		case '2':
			player.setType(2);
			break;
		case '3':
			player.setType(3);
			break;
		case '4':
			player.setType(4);
			break;
		case VK_ESCAPE:
			Game_Stop(hWnd, isStop);
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		break;


	case WM_CHAR:
		switch (wParam) {
		case 'q': case 'Q':
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_TIMER:
	{
		hDC = GetDC(hWnd);
		mDC1 = CreateCompatibleDC(hDC);
		mDC2 = CreateCompatibleDC(mDC1);



		// 이제부터 타이머 ID값으로 분류(1: 게임 업데이트 및 렌더링, 2: 플레이어 깜빡임 3......)

		switch (wParam) {
		case 1: {

			//렌더링 파트
			HBITMAP oldMDC1Bit = (HBITMAP)SelectObject(mDC1, hBitmap);

			bg.Render(mDC1, mDC2, win);												//배경 렌더링


			obsManager.Render_Obstacles(mDC1, cameraY);						// 장애물 렌더링


			playerX = player.getPos().x;
			playerY = player.getPos().y;
			
			if (playerseen)
				player.Render(mDC1, mDC2, playerX, playerY - cameraY);			//플레이어 렌더링






			//UI 파트
			scoreRender(mDC1, cameraY, win);
			fuelRender(mDC1, player.getFuel(), win);


			bg.Camera_Update(player.getPos().y);

			cameraY = bg.GetCameraY();






			//게임 업데이트 파트


			obsManager.AutoAdd(randtype(dre), player.getPos().y); //				AutoAdd(type, playerY)
			obsManager.Update_Obstacles(bg.GetCameraDelta(), cameraY);
			obsManager.Delete_Obstacles(player.getPos().y);



			player.decel();
			player.update();
			player.move(wParam, maxSpeed);

			// 플레이어 히트박스 갱신
			playerRect.left = playerX - player.getSize() / 2;
			playerRect.top = (playerY - cameraY) - player.getSize() / 2;
			playerRect.right = playerX + player.getSize() / 2;
			playerRect.bottom = (playerY - cameraY) + player.getSize() / 2;



			// 충돌 체크
			if (obsManager.Check_PlayerCollision(playerRect) && !player.getishit()) {
				player.setishit(true);
				seencnt = 0;
				SetTimer(hWnd, 2, 15, NULL); //0.5초마다 깜빡임
				player.setHp(player.getHp() - 1);


			}



			// ***************************************************************
			// 플레이어 히트박스 시각화 (파란 사각형) 
			HBRUSH oldBrush = (HBRUSH)SelectObject(mDC1, GetStockObject(NULL_BRUSH));
			HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
			HPEN oldPen = (HPEN)SelectObject(mDC1, hPen);

			Rectangle(mDC1, playerRect.left, playerRect.top, playerRect.right, playerRect.bottom);

			SelectObject(mDC1, oldPen);
			SelectObject(mDC1, oldBrush);
			DeleteObject(hPen);
			// ***************************************************************



			SelectObject(mDC1, oldMDC1Bit);
			DeleteDC(mDC2);
			DeleteDC(mDC1);
			ReleaseDC(hWnd, hDC);


			break;
		}
		case 2: {

			playerseen = !playerseen;
			seencnt++;
			if (seencnt >= 150) {
				player.setishit(false);
				KillTimer(hWnd, 2);
				seencnt = 0;
			}

			break;

		}

		}
		InvalidateRect(hWnd, NULL, FALSE);

		break;
	}

	case WM_PAINT: {
		hDC = BeginPaint(hWnd, &ps);
		mDC1 = CreateCompatibleDC(hDC);

		OldBit[0] = (HBITMAP)SelectObject(mDC1, hBitmap);

		BitBlt(hDC, 0, 0, win.right, win.bottom, mDC1, 0, 0, SRCCOPY);

		escRender(hDC, g_hInst, win, isStop);

		SelectObject(mDC1, OldBit[0]);

		DeleteDC(mDC1);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
		KillTimer(hWnd, 1);			 
		DeleteObject(hBitmap); 
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}