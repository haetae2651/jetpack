#pragma once
#include "Items.h"
#include "resource.h"
#include <math.h>
class Item_HP : public Items
{
	int hitW;
	int hitH;

	float speed = 0;
	HBITMAP hBitmap;
	BITMAP bmp;

public:

	Item_HP(POINT pos, HINSTANCE hInstance) : Items(0, pos, -120) { // Items(int type, POINT pos, int size)
		this->pos = pos;
		setImage(hInstance);


		
	}

	void setImage(HINSTANCE hInstance) override {
		hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP82));
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		Width = bmp.bmWidth;
		Height = bmp.bmHeight;

		hitW = static_cast<int>((Width + size) * 1);
		hitH = static_cast<int>((Height + size) * 1);

	}

	void Update(float cameraDelta, float cameraY, int playerY, int playerX) override{

		hitbox.left = pos.x - size / 2;
		hitbox.top = (pos.y - cameraY) - size / 2;
		hitbox.right = pos.x + hitW + 55;
		hitbox.bottom = (pos.y - cameraY) + hitH + 60;

		//무빙파트

		float vecX = (float)(playerX - pos.x);
		float vecY = (float)(playerY - pos.y);

		float distance = std::sqrt((vecX * vecX) + (vecY * vecY));

		float normX = 0.0f;
		float normY = 0.0f;

		if (distance > 0.0001f) {
			normX = vecX / distance;
			normY = vecY / distance;
		}
		else {
			normX = 0.0f;
			normY = -1.0f;
		}

		float mindis = 350.0f;
		if (distance < mindis)
		{


			float ratio = (mindis - distance) / mindis;
			speed = -(ratio * ratio) * 7.0f;
		}
		else
			speed = 0;
		pos.x += normX * speed;
		pos.y += normY * speed;

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


		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

		Rectangle(hdc, hitbox.left, hitbox.top, hitbox.right, hitbox.bottom);

		SelectObject(hdc, oldPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hPen);
	}

	~Item_HP()
	{
		DeleteObject(hBitmap);
	}

};