#pragma once
#include "Items.h"
class Item_HP : public Items
{
	int hitW = static_cast<int>(Width * 1.1);
	int hitH = static_cast<int>(Height * 1.1);

public:

	Item_HP(POINT pos, HINSTANCE hInstance) : Items(0,pos,30){
		this->pos = pos;
		setImage(hInstance);
		
	}

	void setImage(HINSTANCE hInstance) {

	}

	void Update(float cameraDelta, float cameraY) {
		hitbox.left = pos.x - 1 / Width + 30;
		hitbox.top = (pos.y - cameraY) - 1 / Height + 20;
		hitbox.right = pos.x + Width / 1.25;
		hitbox.bottom = (pos.y - cameraY) + Height / 1.25;
		/*
		frameTimer++;
		if (frameTimer >= 10) {
			frameTimer = 0;
			animCount++;
			animCount %= 2;
		}
		*/
	}

	void Render(HDC hdc, float cameraY) {

	}

	~Item_HP()
	{

	}

};