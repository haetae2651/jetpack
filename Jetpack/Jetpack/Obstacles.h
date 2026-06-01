#pragma once
#include <windows.h>
class Obstacles {

public:
	Obstacles(int type,POINT pos,int size) //생성자
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
	


	virtual void setImage(HINSTANCE hInstance) {
		//장애물 이미지에 ''따라 이미지 설정.
		//픽셀 데이터를 함께 추출해야함. ( 통로 이미지인 경우 )
	}
	
	// 6.2 장애물의 크기에 따라 히트박스 크기 조절해주는 함수 -------------------------------- 근데 위에 setImage 함수가 이 함수임?
	virtual void Set_HitBox(RECT& rect, int bitmapWidth, int bitmapHeight) {
		rect.left = pos.x - bitmapWidth / 2;
		rect.top = pos.y - bitmapHeight / 2;
		rect.right = pos.x + bitmapWidth / 2;
		rect.bottom = pos.y + bitmapHeight / 2;
	}
	
	// 6.2 히트 박스 위치 갱신 함수
	virtual void Update(float cameraDelta, float cameraY) {
		hitbox.left = pos.x - Width / 2;
		hitbox.top = (pos.y - cameraY) - Height / 2;
		hitbox.right = pos.x + Width / 2;
		hitbox.bottom = (pos.y - cameraY) + Height / 2;
	}

	virtual void Render(HDC hdc, float cameraY) {
		// 일단 테스트용, 사각형 그리기
		int screenY = (int)(pos.y - cameraY);
		Rectangle(hdc, pos.x - size, screenY - size,
			pos.x + size, screenY + size);
	}

	virtual ~Obstacles()
	{

	}
	protected:
		POINT pos;
		int size;
		int type; // 장애물 종류 (예: 0 = 작은 돌, 1 = 큰 돌, 2 = 움직이는 장애물. 3 = 통로장애물)
		float speed;
		BITMAP bmp;
		int Width, Height;


		RECT hitbox; // 충돌 판정용 히트박스
};