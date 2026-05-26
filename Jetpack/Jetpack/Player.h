#pragma once
#include <windows.h>
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

};
