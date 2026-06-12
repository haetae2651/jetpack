#pragma once
#include <windows.h>
#include "resource.h"
#include <cmath>
#pragma comment(lib, "msimg32.lib")


class Player {

public:


	
	//get함수
	POINT getPos() { return pos; }
	int getHp() { return hp; }
	int getFuel() { return fuel; }
	bool getishit() { return ishit; }
	int getSize() { return size; }

	//set함수
	void setPos(POINT newPos) { pos = newPos; }
	void setHp(int newHp) { hp = newHp; }
	void setFuel(int newFuel) { fuel = newFuel; }
	void setishit(bool newishit) { ishit = newishit; }


	void move(WPARAM wParam, int maxSpeed,HDC hdc,RECT win) {
		
		float fuelconsume = 0.05f;

		if (Xspeed <= maxSpeed && Xspeed >= -maxSpeed)
		{
			if (fuel > 0) {
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
					Xspeed -= 0.5;
					if (Yspeed > -12.0) Yspeed -= 0.3;

					fuel -= fuelconsume;
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
					Xspeed += 0.5;
					if (Yspeed > -12.0) Yspeed -= 0.3;

					fuel -= fuelconsume;
				}
			}
			RECT hitbox = { pos.x - size/2,500,pos.x + size/2,600};

			if (hitbox.right > win.right || hitbox.left < win.left)
				Xspeed = -Xspeed;
			
		}

	}

	void decel() {
		if (Xspeed > 0) {
			Xspeed -= 0.25;
			if (Xspeed < 0) Xspeed = 0;
		}
		else if (Xspeed < 0) {
			Xspeed += 0.25;
			if (Xspeed > 0) Xspeed = 0;
		}
	}

	void setImage(HINSTANCE hInstance) {
		// Cat (타입1)
		hCat[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
		hCat[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
		hCat[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
		hCat[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP10));
		hCat[4] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP11));
		hCat[5] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP12));
		hCat[6] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP13));
		hCat[7] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP14));
		hCat[8] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP15));
		hCat[9] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP16));

		// Panda (타입2)
		hPanda[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP17));
		hPanda[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP18));
		hPanda[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP19));
		hPanda[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP20));
		hPanda[4] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP21));
		hPanda[5] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP22));
		hPanda[6] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP23));
		hPanda[7] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP24));
		hPanda[8] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP25));
		hPanda[9] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP26));
		hPanda[10] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP27));
		hPanda[11] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP28));

		// Bunny (타입3)
		hBunny[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP55));
		hBunny[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP56));
		hBunny[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP57));
		hBunny[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP58));
		hBunny[4] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP59));
		hBunny[5] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP60));
		hBunny[6] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP61));
		hBunny[7] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP62));
		hBunny[8] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP63));
		hBunny[9] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP64));

		// Penguin (타입4)
		hPenguin[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP65));
		hPenguin[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP66));
		hPenguin[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP67));
		hPenguin[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP68));
		hPenguin[4] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP69));
		hPenguin[5] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP70));
		hPenguin[6] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP71));
		hPenguin[7] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP72));

		// Jetpack 
		hjetpack[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP29));
		hjetpack[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP33));
		hjetpack[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP34));

		// Jetpack Left
		hjetpack[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP35));
		hjetpack[4] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP36));
		hjetpack[5] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP37));
		hjetpack[6] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP38));
		hjetpack[7] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP39));

		// Jetpack Right
		hjetpack[8] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP40));
		hjetpack[9] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP41));
		hjetpack[10] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP42));
		hjetpack[11] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP43));
		hjetpack[12] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP44));

		// 기본 크기 설정 - cat
		BITMAP CatBmp;
		GetObject(hCat[0], sizeof(BITMAP), &CatBmp);
		width = CatBmp.bmWidth;
		height = CatBmp.bmHeight;

		// 판다
		BITMAP PandaBmp;
		GetObject(hPanda[0], sizeof(BITMAP), &PandaBmp);
		width = PandaBmp.bmWidth;
		height = PandaBmp.bmHeight;

		// 버니
		BITMAP BunnyBmp;
		GetObject(hBunny[0], sizeof(BITMAP), &BunnyBmp);
		width = BunnyBmp.bmWidth;
		height = BunnyBmp.bmHeight;
		
		// 펭귄
		BITMAP PenguinBmp;
		GetObject(hPenguin[0], sizeof(BITMAP), &PenguinBmp);
		width = PenguinBmp.bmWidth;
		height = PenguinBmp.bmHeight;

		// 제트팩
		BITMAP jetBmp;
		GetObject(hjetpack[0], sizeof(BITMAP), &jetBmp);
		jetWidth = jetBmp.bmWidth;
		jetHeight = jetBmp.bmHeight;

		//플레이어 이미지 설정
		//플레이어 속도에 따라 각도가 다른 이미지 로드


		// ************************* 이게 무슨 말인지 잘 모르겠사옵니다
		size = static_cast<int>(max(width, height) * 0.2); //렌더링 및 충돌판정	에 사용할 크기

	}

	// 1번 키를 누르면 고양이, 2번 키를 누르면 판다 (임시코드)
	void setType(int type) {
		playerType = type;
		animCount = 0;

		if (type == 1) 
			maxFrame = 10;
		if (type == 2) 
			maxFrame = 12;
		if (type == 3)
			maxFrame = 10;
		if (type == 4)
			maxFrame = 8;
	}






	void Render(HDC mDC1, HDC mDC2, int x, int y) {
		HBITMAP OldBit;
		HBITMAP OldJet = (HBITMAP)SelectObject(mDC2, hjetpack[0]);

		switch (jetState) {
		case 0: // 안누름 → 기본 모형
			OldJet = (HBITMAP)SelectObject(mDC2, hjetpack[0]);
			break;
		case 1: case 2: // 왼쪽 → hjetpack[3~7]
			OldJet = (HBITMAP)SelectObject(mDC2, hjetpack[3 + jetFrame]);
			break;
		case 3: case 4: // 오른쪽 → hjetpack[8~12]
			OldJet = (HBITMAP)SelectObject(mDC2, hjetpack[8 + jetFrame]);
			break;
		case 5: // 동시 → hjetpack[1~2]
			OldJet = (HBITMAP)SelectObject(mDC2, hjetpack[1 + jetFrame]);
			break;
		}

		// 제트팩 회전 적용
		float rad = angle * 3.14159f / 180.0f;
		XFORM xform;
		xform.eM11 = cos(rad);
		xform.eM12 = sin(rad);
		xform.eM21 = -sin(rad);
		xform.eM22 = cos(rad);
		xform.eDx = (float)x;
		xform.eDy = (float)y;

		SetGraphicsMode(mDC1, GM_ADVANCED);
		SetWorldTransform(mDC1, &xform);

		if (playerType == 1) {
			TransparentBlt(mDC1, -jetWidth / 2 - 20, -jetHeight / 2 - 10, jetWidth + 20, jetHeight + 30,
				mDC2, 0, 0, jetWidth, jetHeight, RGB(0, 255, 0));
		}
		else if (playerType == 2) {
			TransparentBlt(mDC1, -jetWidth / 2 - 12, -jetHeight / 2 - 20, jetWidth + 20 , jetHeight + 40,
				mDC2, 0, 0, jetWidth, jetHeight, RGB(0, 255, 0));
		}
		else if (playerType == 3) {
			TransparentBlt(mDC1, -jetWidth / 2 - 12, -jetHeight / 2 - 10, jetWidth + 15, jetHeight + 25,
				mDC2, 0, 0, jetWidth, jetHeight, RGB(0, 255, 0));
		}
		else {
			TransparentBlt(mDC1, -jetWidth / 2 - 12, -jetHeight / 2 - 10, jetWidth + 15, jetHeight + 25,
				mDC2, 0, 0, jetWidth, jetHeight, RGB(0, 255, 0));
		}
		SelectObject(mDC2, OldJet);



		animTimer++;
		if (animTimer >= 10) {
			animTimer = 0;
			animCount = (animCount + 1) % maxFrame;
		}



		if (playerType == 1) {
			OldBit = (HBITMAP)SelectObject(mDC2, hCat[animCount]);
			BITMAP tempBmp;
			GetObject(hCat[animCount], sizeof(BITMAP), &tempBmp);
			width = tempBmp.bmWidth;
			height = tempBmp.bmHeight;
			size = 64;
		}
		else if (playerType == 2) {
			OldBit = (HBITMAP)SelectObject(mDC2, hPanda[animCount]);
			BITMAP tempBmp;
			GetObject(hPanda[animCount], sizeof(BITMAP), &tempBmp);
			width = tempBmp.bmWidth;
			height = tempBmp.bmHeight;
			size = 64; 
		}
		else if (playerType == 3) {
			OldBit = (HBITMAP)SelectObject(mDC2, hBunny[animCount]);
			BITMAP tempBmp;
			GetObject(hBunny[animCount], sizeof(BITMAP), &tempBmp);
			width = tempBmp.bmWidth;
			height = tempBmp.bmHeight;
			size = 64;
		}
		else {
			OldBit = (HBITMAP)SelectObject(mDC2, hPenguin[animCount]);
			BITMAP tempBmp;
			GetObject(hPenguin[animCount], sizeof(BITMAP), &tempBmp);
			width = tempBmp.bmWidth;
			height = tempBmp.bmHeight;
			size = 64;
		}


		TransparentBlt(mDC1, -size / 2, -size / 2, size, size,
			mDC2, 0, 0, width, height, RGB(0, 255, 0));
		SelectObject(mDC2, OldBit);

		// 변환 초기화
		ModifyWorldTransform(mDC1, NULL, MWT_IDENTITY);
		SetGraphicsMode(mDC1, GM_COMPATIBLE);
	}



	void update() {						
		Yspeed += 0.2; // 중력 - ***********유하영이 추가했다***********
		pos.x += static_cast<int>(Xspeed);
		pos.y += static_cast<int>(Yspeed);

		updateJet(); // 제트팩 애니메이션 업데이트

		angle = (float)Xspeed * 3.0f;
		if (angle > 30.0f)  angle = 30.0f;
		if (angle < -30.0f) angle = -30.0f;





	}



	// 좌우 키에 따라 달라지는 애니메이션
	void updateJet() {
		bool isLeftPressed = (GetAsyncKeyState(VK_LEFT) & 0x8000);
		bool isRightPressed = (GetAsyncKeyState(VK_RIGHT) & 0x8000);

		switch (jetState) {
		case 0: // 안누름
			if (isLeftPressed && isRightPressed) {
				jetState = 5; jetFrame = 0; jetTimer = 0;
			}
			else if (isLeftPressed) {
				jetState = 1; jetFrame = 0; jetTimer = 0;
			}
			else if (isRightPressed) {
				jetState = 3; jetFrame = 0; jetTimer = 0;
			}
			break;

		case 1: // 왼쪽 발사중 (3~6프레임)
			if (!isLeftPressed && !isRightPressed) {
				jetState = 0; jetFrame = 0; jetTimer = 0; break;
			}
			if (isLeftPressed && isRightPressed) {
				jetState = 5; jetFrame = 0; jetTimer = 0; break;
			}
			jetTimer++;
			if (jetTimer >= 10) {
				jetTimer = 0;
				jetFrame++;
				if (jetFrame >= 4) {
					jetState = 2; jetFrame = 4;
				}
			}
			break;

		case 2: // 왼쪽 유지중 (6~7프레임)
			if (!isLeftPressed && !isRightPressed) {
				jetState = 0; jetFrame = 0; jetTimer = 0; break;
			}
			if (isLeftPressed && isRightPressed) {
				jetState = 5; jetFrame = 0; jetTimer = 0; break;
			}
			jetTimer++;
			if (jetTimer >= 10) {
				jetTimer = 0;
				jetFrame++;
				if (jetFrame >= 5) jetFrame = 4;
			}
			break;

		case 3: // 오른쪽 발사중 (8~11프레임)
			if (!isLeftPressed && !isRightPressed) {
				jetState = 0; jetFrame = 0; jetTimer = 0; break;
			}
			if (isLeftPressed && isRightPressed) {
				jetState = 5; jetFrame = 0; jetTimer = 0; break;
			}
			jetTimer++;
			if (jetTimer >= 10) {
				jetTimer = 0;
				jetFrame++;
				if (jetFrame >= 4) {
					jetState = 4; jetFrame = 4;
				}
			}
			break;

		case 4: // 오른쪽 유지중 (11~12프레임 반복)
			if (!isLeftPressed && !isRightPressed) {
				jetState = 0; jetFrame = 0; jetTimer = 0; break;
			}
			if (isLeftPressed && isRightPressed) {
				jetState = 5; jetFrame = 0; jetTimer = 0; break;
			}
			jetTimer++;
			if (jetTimer >= 10) {
				jetTimer = 0;
				jetFrame++;
				if (jetFrame >= 5) jetFrame = 4;
			}
			break;

		case 5: // 동시 (1~2프레임 반복)
			if (!isLeftPressed && !isRightPressed) {
				jetState = 0; jetFrame = 0; jetTimer = 0; break;
			}
			jetTimer++;
			if (jetTimer >= 10) {
				jetTimer = 0;
				jetFrame++;
				if (jetFrame >= 2) jetFrame = 0;
			}
			break;
		}
	}


private:

	int hp = 5;
	float fuel = 100;
	POINT pos;


	double Yspeed = 0;
	double Xspeed = 0;

	HBITMAP hBitmap;
	BITMAP bmp;
	int width, height;
	int size;

	// 플레이어 타입
	int playerType = 1; // 1 = 기본, 2 = 판다, 3 = 토끼, 4 = 펭귄

	// 플레이어 외형
	HBITMAP hCat[10];
	HBITMAP hPanda[12];
	HBITMAP hBunny[10];
	HBITMAP hPenguin[8];

	// 제트팩
	HBITMAP hjetpack[13];

	float angle = 0.0f; // 기울기

	int jetWidth, jetHeight;

	int jetState = 0; // 0 = 안누름, 1 = 발사중(1~4), 2 = 유지중(5~6)
	int jetFrame = 0;
	int jetTimer = 0;

	int animCount = 0;
	int animTimer = 0;
	int maxFrame = 10;

	bool ishit = false;
};
