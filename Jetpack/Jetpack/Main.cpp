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
#include "OBS_Following.h"

#include "Items.h"
#include "ItemsManager.h"

#include "Item_HP.h"

#include "IngameUI.h"
#include "PregameUI.h"
#include "fade.h"
using namespace std;

default_random_engine dre{ random_device{}() };

uniform_int_distribution<int> randtype(0, 10);


int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
float cameraY;

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
	static RECT playerRect;		//충돌 히트박스


	// 임시 테스트용 장애물
	static Obstacles tempObs{ 0, {1000, -10}, 100 };
	static int obsX, obsY;
	static int obsSize = tempObs.getSize();

	//장애물 관련 변수
	static ObstacleManager obsManager; // 5.27
	static BackGround bg;

	//아이템 관련 변수
	static ItemsManager itemsManager;


	//UI 관련 변수
	static bool IngameUI_Render = false;
	static int selected = 0;
	static bool isFadeIn = false;
	static bool isGameOverFade = false;
	static bool isTitleFadeIn = true;
	static bool isIngame = false;

	//6.2 게임설정
	static bool isStop = false; // 게임 멈추기
	static bool timer1init = false;

	static int selectedChar = 0; // 0=Cat, 1=Panda, 2=Bunny, 3=Penguin

	//소리변수
	static FMOD::System* pMainBgmSystem = nullptr;
	static FMOD::Sound* pTitleBGM = nullptr; // Timer 3용 (IDR_WAVE4)
	static FMOD::Sound* pGameBGM = nullptr;  // Timer 1용 (IDR_WAVE5)
	static FMOD::Channel* pBgmChannel = nullptr;
	static FMOD::Sound* pKeySound = nullptr;   // 좌우 키 입력음 (IDR_WAVE5)
	static FMOD::Sound* pEnterSound = nullptr; // 엔터 선택음 (IDR_WAVE6)

	switch (uMsg) {
	case WM_CREATE:
	{

		FMOD::System_Create(&pMainBgmSystem);
		pMainBgmSystem->init(32, FMOD_INIT_NORMAL, nullptr);

		// IDR_WAVE4 (타이틀 브금) 로드 - FMOD_LOOP_NORMAL로 반복 재생
		HRSRC hRes4 = FindResource(g_hInst, MAKEINTRESOURCE(IDR_WAVE4), TEXT("WAVE"));
		if (hRes4) {
			HGLOBAL hData = LoadResource(g_hInst, hRes4);
			DWORD size = SizeofResource(g_hInst, hRes4);
			FMOD_CREATESOUNDEXINFO ex = { 0 };
			ex.cbsize = sizeof(FMOD_CREATESOUNDEXINFO); ex.length = size;
			pMainBgmSystem->createSound((const char*)LockResource(hData), FMOD_OPENMEMORY | FMOD_LOOP_NORMAL, &ex, &pTitleBGM);
		}

		// IDR_WAVE5 (인게임 브금) 로드
		HRSRC hRes5 = FindResource(g_hInst, MAKEINTRESOURCE(IDR_WAVE5), TEXT("WAVE"));
		if (hRes5) {
			HGLOBAL hData = LoadResource(g_hInst, hRes5);
			DWORD size = SizeofResource(g_hInst, hRes5);
			FMOD_CREATESOUNDEXINFO ex = { 0 };
			ex.cbsize = sizeof(FMOD_CREATESOUNDEXINFO); ex.length = size;
			pMainBgmSystem->createSound((const char*)LockResource(hData), FMOD_OPENMEMORY | FMOD_LOOP_NORMAL, &ex, &pGameBGM);
		}

		// 방향키 입력음 (IDR_WAVE6) 로드

		HRSRC hResKey = FindResource(g_hInst, MAKEINTRESOURCE(IDR_WAVE7), TEXT("WAVE"));
		if (hResKey) {
			HGLOBAL hData = LoadResource(g_hInst, hResKey);
			DWORD size = SizeofResource(g_hInst, hResKey);
			FMOD_CREATESOUNDEXINFO ex = { 0 };
			ex.cbsize = sizeof(FMOD_CREATESOUNDEXINFO); ex.length = size;
			// FMOD_DEFAULT 사용 (반복 안 함)
			pMainBgmSystem->createSound((const char*)LockResource(hData), FMOD_OPENMEMORY | FMOD_DEFAULT, &ex, &pKeySound);
		}

		// 엔터 입력음 (IDR_WAVE7) 로드
		HRSRC hResEnter = FindResource(g_hInst, MAKEINTRESOURCE(IDR_WAVE6), TEXT("WAVE"));
		if (hResEnter) {
			HGLOBAL hData = LoadResource(g_hInst, hResEnter);
			DWORD size = SizeofResource(g_hInst, hResEnter);
			FMOD_CREATESOUNDEXINFO ex = { 0 };
			ex.cbsize = sizeof(FMOD_CREATESOUNDEXINFO); ex.length = size;
			// FMOD_DEFAULT 사용 (반복 안 함)
			pMainBgmSystem->createSound((const char*)LockResource(hData), FMOD_OPENMEMORY | FMOD_DEFAULT, &ex, &pEnterSound);
		}

		// 프로그램 켜지자마자 타이틀화면(Timer 3)이므로 타이틀 브금 재생 시작
		if (pMainBgmSystem && pTitleBGM) {
			pMainBgmSystem->playSound(pTitleBGM, 0, false, &pBgmChannel);
		}



		hDC = GetDC(hWnd);
		mDC1 = CreateCompatibleDC(hDC);
		mDC2 = CreateCompatibleDC(mDC1);

		IngameUI_Render = true;


		fadeset_in(win);
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &win);

		SetTimer(hWnd, 3, 1, NULL);

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


		
		obsManager.setWin(win);

		

		//아이템 초기화

		itemsManager.Add_Item(new Item_HP({ 500,-500 }, g_hInst));
		itemsManager.setWin(win);

		//UI 초기화
		setUI(g_hInst);
		pre_setUI(g_hInst);;

		//초기 렌더링

		/*
		HBITMAP oldMDC1Bit = (HBITMAP)SelectObject(mDC1, hBitmap);
		bg.Render(mDC1, mDC2, win);												//배경 렌더링
		obsManager.Render_Obstacles(mDC1, cameraY);						// 장애물 렌더링
		playerX = player.getPos().x;
		playerY = player.getPos().y;
		if (playerseen)
			player.Render(mDC1, mDC2, playerX, playerY - cameraY);			//플레이어 렌더링
		itemsManager.Render_Items(mDC1, cameraY);

		TCHAR str[200];
		wsprintf(str, L"SPACE키를 눌러 시작(PregameUI를 위해 임시로 해놨다.)");

		TextOut(mDC1, 500, 600, str, wcslen(str));
		SelectObject(mDC1, oldMDC1Bit);
		DeleteDC(mDC2);
		DeleteDC(mDC1);

		*/



		SetTimer(hWnd, 3, 1, NULL);
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseDC(hWnd, hDC);
		break;
	}

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
		case VK_ESCAPE:
			Game_Stop(hWnd, isStop);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case VK_RIGHT: {
			selected++;
			if (selected >= 2)
				selected = 0;
			selectedChar++;
			if (selectedChar >= 4)
				selectedChar = 0;

			if (pMainBgmSystem && pKeySound && !isIngame) {
				pMainBgmSystem->playSound(pKeySound, 0, false, nullptr);
			}

			break;
		}

		case VK_LEFT: {
			selected--;
			if (selected <= -1)
				selected = 1;
			selectedChar--;
			if (selectedChar <= -1)
				selectedChar = 3;

			if (pMainBgmSystem && pKeySound&&!isIngame) {
				pMainBgmSystem->playSound(pKeySound, 0, false, nullptr);
			}
			break;
		}

		}

		break;


	case WM_CHAR:
		switch (wParam) {
		case 'q': case 'Q': {
			PostQuitMessage(0);
			break;
		}
	
		case 'r': case 'R':{
			if (isStop)
			{

				timer1init = false;
			}
			break;
		}

	
		}


	case WM_TIMER:
	{
		hDC = GetDC(hWnd);
		mDC1 = CreateCompatibleDC(hDC);
		mDC2 = CreateCompatibleDC(mDC1);

		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && selected == 1 && !isTitleFadeIn)
		{
			PostQuitMessage(0);

		}

		// 이제부터 타이머 ID값으로 분류(1: 게임 업데이트 및 렌더링, 2: 플레이어 깜빡임 3: 타이틀화면UI

		switch (wParam) {
		case 1: {

			if (!timer1init)
			{

				if (pBgmChannel) pBgmChannel->stop();
				if (pMainBgmSystem && pGameBGM) {
					pMainBgmSystem->playSound(pGameBGM, 0, false, &pBgmChannel);
				}

				itemsManager.Clear();
				obsManager.Clear();
				player.setFuel(100);
				player.setHp(5);
				player.setSpeed(0,0);
				player.setPos({ win.right / 2, win.bottom / 2 });
				playerX = player.getPos().x;
				playerY = player.getPos().y;
				player.setishit(false);
				playerseen = true;
				isStop = false;
				timer1init = true;

				KillTimer(hWnd, 2);
				seencnt = 0;
				bg.Camera_Init(win.bottom);
				cameraY = bg.GetCameraY();
				isStop = false;
				timer1init = true;
				isIngame = true;
			}

			//렌더링 파트



			HBITMAP oldMDC1Bit = (HBITMAP)SelectObject(mDC1, hBitmap);

			bg.Render(mDC1, mDC2, win);												//배경 렌더링


			obsManager.Render_Obstacles(mDC1, cameraY);						// 장애물 렌더링


			playerX = player.getPos().x;
			playerY = player.getPos().y;
			
			if (playerseen)
				player.Render(mDC1, mDC2, playerX, playerY - cameraY);			//플레이어 렌더링



			itemsManager.Render_Items(mDC1, cameraY);						// 아이템 렌더링


			//UI 파트
			scoreRender(mDC1, cameraY, win);
			fuelRender(mDC1, player.getFuel(), win);
			HeartRender(mDC1, player.getHp(), win);

			bg.Camera_Update(player.getPos().y);

			cameraY = bg.GetCameraY();






			//게임 업데이트 파트
			if (!isStop)
			{



				obsManager.AutoAdd(randtype(dre), player.getPos().y, playerX); //				AutoAdd(type, playerY)
				obsManager.Update_Obstacles(bg.GetCameraDelta(), cameraY);
				obsManager.Delete_Obstacles(player.getPos().y);

				itemsManager.Update_Items(bg.GetCameraDelta(), cameraY, playerY, playerX);
				itemsManager.Delete_Items(player.getPos().y);
				itemsManager.AutoAdd(randtype(dre), player.getPos().y);

				player.decel();
				player.update();
				player.move(wParam, maxSpeed, mDC1, win);



				// 플레이어 히트박스 갱신
				playerRect.left = playerX - player.getSize() / 2 + 10;
				playerRect.top = (playerY - cameraY) - player.getSize() / 2 + 10;
				playerRect.right = playerRect.left + 40;
				playerRect.bottom = playerRect.top + 50;


				// 충돌 체크
				if (obsManager.Check_PlayerCollision(playerRect, cameraY) && !player.getishit()) {
					player.setishit(true);
					seencnt = 0;
					SetTimer(hWnd, 2, 15, NULL); //0.5초마다 깜빡임
					player.setHp(player.getHp() - 1);
					player.Hitsound();
					player.setSpeed(player.getXSpeed() * -1, player.getYSpeed() * -1);
					player.setFuel(player.getFuel() - 10);


				}

				int hitItemType = itemsManager.Check_And_Eat_Item(playerRect);
				int maxHP = 5;
				if (hitItemType != -1) // -1이 아니라면 무언가 아이템과 충돌하여 먹은 상태
				{
					player.Itemsound();
					if (hitItemType == 0) {
						player.setHp(player.getHp() + 1);
						if (player.getHp() > maxHP)
							player.setHp(maxHP);

						player.setFuel((100.0f - player.getFuel()) * 0.1 + 20 + player.getFuel());
						if (player.getFuel() > 100)
							player.setFuel(100);

					}
					else if (hitItemType == 1) {

						player.setFuel(player.getFuel() + 80);
						if (player.getFuel() > 100)
							player.setFuel(100);
					}


					// 게임 오버

					

				}

				if (player.getHp() < 1 || player.getPos().y - cameraY > win.bottom + 100)
				{
					if (!isGameOverFade)
					{
						isStop = false;
						fadeset(g_hInst, win);
						isGameOverFade = true;
					}
				}


			}
			escRender(mDC1, g_hInst, win, isStop);

			

			if (isFadeIn) {
				if (fadein_update(mDC1)) {
					isFadeIn = false; // 완전히 화면이 걷히면 더 이상 그리지 않음
				}
			}

			if (isGameOverFade)
			{
				bool isFadeFinished = fadeout_update(mDC1);

				if (isFadeFinished)
				{

					if (pBgmChannel) pBgmChannel->stop();
					if (pMainBgmSystem && pTitleBGM) {
						pMainBgmSystem->playSound(pTitleBGM, 0, false, &pBgmChannel);
					}

					isGameOverFade = false;
					timer1init = false; 

					KillTimer(hWnd, 1); 
					KillTimer(hWnd, 2);
					fadeset_in(win);
					isTitleFadeIn = true;
					SetTimer(hWnd, 3, 1, NULL); 
					timer1init = false;
				}
			}

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
		case 3:
		{
			isIngame = false;
			HBITMAP oldMDC1Bit = (HBITMAP)SelectObject(mDC1, hBitmap);

			staticUIRender(mDC1, win);
			dynamicUIRender(mDC1, win, hWnd,selected);

			if (isTitleFadeIn) {
				if (fadein_update(mDC1)) {
					isTitleFadeIn = false; // 완전히 밝아지면 업데이트 중지
				}
			}

			SelectObject(mDC1, oldMDC1Bit);


			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && selected == 0 && !isTitleFadeIn)
			{
				KillTimer(hWnd, 3);
				fadeset(g_hInst, win);
				SetTimer(hWnd, 4, 1, NULL);
			}

			DeleteDC(mDC2);
			DeleteDC(mDC1);
			ReleaseDC(hWnd, hDC);
			break;
		}
		case 4:
		{
			HBITMAP oldMDC1Bit = (HBITMAP)SelectObject(mDC1, hBitmap);

			staticUIRender(mDC1, win);
			dynamicUIRender(mDC1, win, hWnd, selected);

			bool isFadeFinished = fadeout_update(mDC1);

			SelectObject(mDC1, oldMDC1Bit);
			DeleteDC(mDC2);
			DeleteDC(mDC1);
			ReleaseDC(hWnd, hDC);

			if (isFadeFinished)
			{
				KillTimer(hWnd, 4);          
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);

				fadeset_in(win);
				isFadeIn = true;
				SetTimer(hWnd, 5, 1, NULL); // 캐릭터선택화면으로 넘어가기
			}
			break;
		}
		case 5:
		{
			HBITMAP oldMDC1Bit = (HBITMAP)SelectObject(mDC1, hBitmap);

			charSelectRender(mDC1, win, selectedChar);

			if (isFadeIn) {
				if (fadein_update(mDC1)) {
					isFadeIn = false;
				}
			}

			SelectObject(mDC1, oldMDC1Bit);
			DeleteDC(mDC2);
			DeleteDC(mDC1);
			ReleaseDC(hWnd, hDC);

			if (GetAsyncKeyState(VK_RETURN) & 0x8000 && !isFadeIn)
			{
				if (pMainBgmSystem && pEnterSound) {
					pMainBgmSystem->playSound(pEnterSound, 0, false, nullptr);
				}

				player.setType(selectedChar + 1); // 1~4
				KillTimer(hWnd, 5);
				fadeset(g_hInst, win);
				SetTimer(hWnd, 6, 1, NULL);
			}
			break;
		}
		case 6:
		{
			HBITMAP oldMDC1Bit = (HBITMAP)SelectObject(mDC1, hBitmap);

			charSelectRender(mDC1, win, selectedChar);

			bool isFadeFinished = fadeout_update(mDC1);

			SelectObject(mDC1, oldMDC1Bit);
			DeleteDC(mDC2);
			DeleteDC(mDC1);
			ReleaseDC(hWnd, hDC);

			if (isFadeFinished)
			{
				KillTimer(hWnd, 6);
				fadeset_in(win);
				isFadeIn = true;
				timer1init = false;
				SetTimer(hWnd, 1, 1, NULL); // 게임 시작
			}
			break;
		}
		}

		if (pMainBgmSystem) {
			pMainBgmSystem->update();
		}

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
		KillTimer(hWnd, 1);			 
		DeleteObject(hBitmap); 
		ReleaseUI();
		PostQuitMessage(0);

		if (pTitleBGM) pTitleBGM->release();
		if (pGameBGM) pGameBGM->release();

		if (pKeySound) pKeySound->release();
		if (pEnterSound) pEnterSound->release();

		if (pMainBgmSystem) {
			pMainBgmSystem->close();
			pMainBgmSystem->release();
		}


		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}