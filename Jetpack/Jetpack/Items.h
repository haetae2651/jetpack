#pragma once
#include <windows.h>
class Items {

public:
	Items(int type, POINT pos, int size) //생성자
	{
		this->type = type;
		this->pos = pos;
		this->size = size;


	}


	//get함수
	int getType() { return type; }
	POINT getPos() { return pos; }
	int getSize() { return size; }
	int getWidth() { return Width; }
	int getHeight() { return Height; }

	RECT getHitBox() { return hitbox; }

	//set함수

	void setPos(POINT newPos) { this->pos = newPos; }

	
	// 가상함수들
	virtual void setImage(HINSTANCE hInstance)  = 0;

	virtual void Update(float cameraDelta, float cameraY, int playerY, int playerX) = 0;

	virtual void Render(HDC hdc, float cameraY) = 0;

	virtual ~Items()
	{
		
	}
protected:
	POINT pos;
	int size;
	int type; // 아이템 종류 (0 = 생명력팩,2 = 연료)
	float speed;
	BITMAP bmp;
	int Width, Height;


	RECT hitbox; // 충돌 판정용 히트박스
};