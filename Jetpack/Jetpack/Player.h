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


	void move(WPARAM wParam, int maxSpeed) {			// 좌우 누르면 속도 증가
		

		// GetAsyncKeyState는 현재 키의 상태를 반환하는 함수 (KeyDown으로 처리하면 버벅거림)

		if (Xspeed <= maxSpeed && Xspeed >= -maxSpeed)
		{

			//왼쪽키: 우측으로 이동, 오른쪽키: 좌측으로 이동 (누른 키 방향의 로켓이 켜지기 떄문에 반대로 이동)
			if(GetAsyncKeyState(VK_RIGHT) & 0x8000) {
				Xspeed -= 0.5;
			}
			if( GetAsyncKeyState(VK_LEFT) & 0x8000) {
				Xspeed += 0.5;
			}

		}
		


		
		

	}

	void decel() {						//감속
		if (Xspeed > 0) {
			Xspeed -= 0.25;
			if (Xspeed < 0) Xspeed = 0;
		}
		else if (Xspeed < 0) {
			Xspeed += 0.25;
			if (Xspeed > 0) Xspeed = 0;
		}
	}

	void update() {						//위치 업데이트 (현재 스피드 만큼 위치에 더함)
		pos.x += static_cast<int>(Xspeed);
		pos.y += static_cast<int>(Yspeed);
	}




private:

	int hp = 1;
	int fuel = 100;
	POINT pos;


	double Yspeed = 0;
	double Xspeed = 0;

};
