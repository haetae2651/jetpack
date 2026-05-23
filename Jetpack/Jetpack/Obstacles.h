#pragma once
#include <windows.h>
class Obstacles {

public:
	Obstacles(int type,POINT pos,int size) //생성자
	{
		this->type = type;
		this->pos = pos;
		this->size = size;


		this->hBitmap = NULL;
		this->pixelData = nullptr;
		this->width = 0;
		this->height = 0;
		SetRect(&this->boundingBox, 0, 0, 0, 0);
	}


	//get함수
	int getType() { return type; }
	POINT getPos() { return pos; }
	int getSize() { return size; }

	//set함수
	


	void setImage(HINSTANCE hInstance) {
		//장애물 이미지에 ''따라 이미지 설정.
		//픽셀 데이터를 함께 추출해야함.
	}
	
	// ********** ObstacleNode.h에서 쓰일 함수 **********
	void Update() {
		pos.y += speed;  // 장애물이 아래로 이동
		// type에 따라 움직임 다르게 처리 가능
	}

	void Render(HDC hdc, float cameraY) {
		// 일단 테스트용으로 사각형 그리기
		int screenY = (int)(pos.y - cameraY);
		Rectangle(hdc, pos.x - size, screenY - size,
			pos.x + size, screenY + size);
	}

	~Obstacles()
	{

	}
	private:
		POINT pos;
		int size;
		int type; // 장애물 종류 (예: 0 = 작은 돌, 1 = 큰 돌, 2 = 움직이는 장애물. 3 = 통로장애물)
		int speed;




		// 충돌판지를 위한 변수들
		HBITMAP hBitmap;       // 로드된 비트맵의 GDI 핸들
		COLORREF* pixelData;   // 비트맵의 픽셀 색상 정보가 담길 동적 배열 포인터
		int width;             // 비트맵 이미지의 실제 가로 픽셀 크기
		int height;            // 비트맵 이미지의 실제 세로 픽셀 크기
		RECT boundingBox;      // 1차 필터링(AABB 사각형 충돌)을 위한 외곽 사각형
};