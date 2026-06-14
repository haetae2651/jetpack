#pragma once
#include "Items.h"
#include "resource.h"
#include <math.h>

class Item_Fuel : public Items
{
	int hitW;
	int hitH;
	RECT win;
	float speed = 0;
	float k = 0.0f;

	int startX;
	bool isInitialized = false;

	HBITMAP hBitmap;
	BITMAP bmp;

	std::uniform_int_distribution<int> randoffset;
	std::default_random_engine dre{ std::random_device{}() };


public:

	Item_Fuel(POINT pos, HINSTANCE hInstance) : Items(1, pos, -120) { // Items(int type, POINT pos, int size)
		this->pos = pos;
		setImage(hInstance);



	}

	void setWin(RECT win) override{
		this->win = win;

		randoffset.param(std::uniform_int_distribution<int>::param_type(-1, 1));

	}

	void setImage(HINSTANCE hInstance) override {
		hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP83));
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		Width = bmp.bmWidth;
		Height = bmp.bmHeight;

		hitW = static_cast<int>((Width + size) * 1);
		hitH = static_cast<int>((Height + size) * 1);

	}

	void Update(float cameraDelta, float cameraY, int playerY, int playerX) override {

		if (!isInitialized) {
			startX = pos.x;
			isInitialized = true;
			k = randoffset(dre);
		}


		hitbox.left = pos.x - size / 2;
		hitbox.top = (pos.y - cameraY) - size / 2;
		hitbox.right = pos.x + hitW + 55;
		hitbox.bottom = (pos.y - cameraY) + hitH + 60;

		//무빙파트
		if (win.right > 0) {
			float amplitude = (win.right / 2.0f) - 300.0f;

			pos.x = startX + static_cast<int>(sin(k) * amplitude);

			k += 0.02f;
		}
	}

	void Render(HDC hdc, float cameraY) override {


		int screenY = (int)(pos.y - cameraY);

		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		Width = bmp.bmWidth;
		Height = bmp.bmHeight;


		TransparentBlt(hdc, pos.x - size / 2, screenY - size / 2, size + Width - 5, size + Height,
			hMemDC, 0, 0, Width, Height, RGB(0, 255, 0));


		SelectObject(hMemDC, oldBit);
		DeleteDC(hMemDC);


		
	}

	~Item_Fuel()
	{
		DeleteObject(hBitmap);
	}

};