#pragma once
#include <windows.h>
#include "resource.h"
#include "Obstacles.h"
#include <string>



class OBS_Random : public Obstacles {
private:
    HBITMAP hBitAnim[3];
    int animCount = 0;
    //int chaWidth, chaHeight;
   // int size = 40;                    <- 상위클래스의 변수에 있다.
    int frameTimer = 10;

public :
	OBS_Random(POINT pos, HINSTANCE hInstance) : Obstacles(0, pos, 20) // Obstacles(int type, POINT pos, int size)
	{
		speed = 2;
        hBitAnim[0] = hBitAnim[1] = hBitAnim[2] = NULL;
        Width = Height = 0;
        setImage(hInstance);
        size = 40;
        //여기다가 히트박스 크기 초기화 하면 될듯. 아니면 다른데다가.
        //히트박스는 이미지 크기에 맞춰서? 예를들어 이미지의 80% 정도가 히트박스가 되도록.

	}

    ~OBS_Random() {
        for (int i = 0; i < 3; i++)
            DeleteObject(hBitAnim[i]);
    }

    void setImage(HINSTANCE hInstance) override {
        hBitAnim[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
        hBitAnim[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
        hBitAnim[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));

    }

    void Update(float cameraDelta) override {


        frameTimer++;
        if (frameTimer >= 10)
        {
            frameTimer = 0;

            animCount++;
            animCount %= 3;
        }
    }
    void Render(HDC hdc, float cameraY) override {
        int screenY = (int)(pos.y - cameraY);

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hBitAnim[animCount]); 
        GetObject(hBitAnim[animCount], sizeof(BITMAP), &bmp);
        Width = bmp.bmWidth;
        Height = bmp.bmHeight;


        TransparentBlt(hdc, pos.x - size / 2, screenY - size / 2, size + Width, size + Height,
            hMemDC, 0, 0, Width, Height, RGB(0, 255, 0));


        SelectObject(hMemDC, oldBit);
        DeleteDC(hMemDC);
    }
};