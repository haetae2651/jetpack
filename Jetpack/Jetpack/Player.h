#pragma once
#include <windows.h>
#include "resource.h"
#pragma comment(lib, "msimg32.lib")


class Player {

public:


	
	//get함수
	POINT getPos() { return pos; }
	int getHp() { return hp; }
	int getFuel() { return fuel; }

	//set함수
	void setPos(POINT newPos) { pos = newPos; }
	void setHp(int newHp) { hp = newHp; }
	void setFuel(int newFuel) { fuel = newFuel; }


	void move(WPARAM wParam, int maxSpeed) {
		


		if (Xspeed <= maxSpeed && Xspeed >= -maxSpeed)
		{

			if(GetAsyncKeyState(VK_RIGHT) & 0x8000) {
				Xspeed -= 0.5;
				if (Yspeed > -10.0) Yspeed -= 0.4;

				fuel -= 0.1f;
			}
			if( GetAsyncKeyState(VK_LEFT) & 0x8000) {
				Xspeed += 0.5;
				if (Yspeed > -10.0) Yspeed -= 0.4;

				fuel -= 0.1f;
			}

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

		//플레이어 이미지 설정
		//플레이어 속도에 따라 각도가 다른 이미지 로드

		hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(TestImage));
		GetObject(hBitmap, sizeof(bmp), &bmp);
		width = bmp.bmWidth;
		height = bmp.bmHeight;

		size = static_cast<int>(max(width, height) * 0.2); //렌더링 및 충돌판정	에 사용할 크기

	}

	void Render(HDC mDC1, HDC mDC2 , int x, int y) {

		HBITMAP OldBit = (HBITMAP)SelectObject(mDC2, hBitmap); // 비트맵과 메모리 DC 연결
		TransparentBlt(mDC1, x - size / 2, y - size / 2, size, size,
			mDC2, 0, 0, width, height, RGB(0, 255,0));
		SelectObject(mDC2, OldBit);

	}

	void update() {						
		Yspeed += 0.2; // 중력 - ***********유하영이 추가했다***********
		pos.x += static_cast<int>(Xspeed);
		pos.y += static_cast<int>(Yspeed);
	}




private:

	int hp = 1;
	float fuel = 100;
	POINT pos;


	double Yspeed = 0;
	double Xspeed = 0;

	HBITMAP hBitmap;
	BITMAP bmp;
	int width, height;
	int size;
};
