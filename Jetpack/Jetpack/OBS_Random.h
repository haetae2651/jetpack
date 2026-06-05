#pragma once
#include <windows.h>
#include "resource.h"
#include "Obstacles.h"
#include <string>



class OBS_Random : public Obstacles {
private:
    HBITMAP hBitAnim[2];
    int animCount = 0;
    //int chaWidth, chaHeight;
   // int size = 40;                    <- 상위클래스의 변수에 있다.
    int frameTimer = 10;

    BITMAP bmp;

    int hitW = Width * 0.8;
    int hitH = Height * 0.8;
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

        hitbox.left = pos.x - hitW / 2;
        hitbox.top = pos.y - hitH / 2;
        hitbox.right = pos.x + hitW / 2;
        hitbox.bottom = pos.y + hitH / 2;
	}

    ~OBS_Random() {
        for (int i = 0; i < 2; i++)
            DeleteObject(hBitAnim[i]);
    }

    void setImage(HINSTANCE hInstance) override {
        hBitAnim[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
        hBitAnim[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));

        GetObject(hBitAnim[0], sizeof(BITMAP), &bmp);
        Width = bmp.bmWidth;
        Height = bmp.bmHeight;
    }

    void Update(float cameraDelta, float cameraY) override {
        // 카메라 위치에 따라 히트박스도 달라져야하니 다시 초기화
        hitbox.left = pos.x - 1/Width + 30;
        hitbox.top = (pos.y - cameraY) - 1/Height + 20;
        hitbox.right = pos.x + Width / 1.25;
        hitbox.bottom = (pos.y - cameraY) + Height / 1.25;

        frameTimer++;
        if (frameTimer >= 10) {
            frameTimer = 0;
            animCount++;
            animCount %= 2;
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


        // 히트박스 범위 보이는 코드 - ai썻음 ㅎ
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

        Rectangle(hdc, hitbox.left, hitbox.top, hitbox.right, hitbox.bottom);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hPen);
    }
};