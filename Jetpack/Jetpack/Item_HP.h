#pragma once
#include "Items.h"
#include "resource.h"
class Item_HP : public Items
{
	int hitW;
	int hitH;

	HBITMAP hBitmap;
	BITMAP bmp;

public:

	Item_HP(POINT pos, HINSTANCE hInstance) : Items(0,pos,30){
		this->pos = pos;
		setImage(hInstance);


		
	}

	void setImage(HINSTANCE hInstance) override {
		hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP82));
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		Width = bmp.bmWidth;
		Height = bmp.bmHeight;

		hitW = static_cast<int>(Width * 1.1);
		hitH = static_cast<int>(Height * 1.1);

	}

	void Update(float cameraDelta, float cameraY) override{
		hitbox.left = pos.x - 1 / Width + 30;
		hitbox.top = (pos.y - cameraY) - 1 / Height + 20;
		hitbox.right = pos.x + Width / 1.25;
		hitbox.bottom = (pos.y - cameraY) + Height / 1.25;

	}

	void Render(HDC hdc, float cameraY) override {


		int screenY = (int)(pos.y - cameraY);

		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		Width = bmp.bmWidth;
		Height = bmp.bmHeight;


		TransparentBlt(hdc, pos.x - size / 2, screenY - size / 2, size + Width, size + Height,
			hMemDC, 0, 0, Width, Height, RGB(0, 255, 0));


		SelectObject(hMemDC, oldBit);
		DeleteDC(hMemDC);


		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

		Rectangle(hdc, hitbox.left, hitbox.top, hitbox.right, hitbox.bottom);

		SelectObject(hdc, oldPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hPen);
	}

	~Item_HP()
	{

	}

};