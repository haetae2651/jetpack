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
	OBS_Random(POINT pos, HINSTANCE hInstance) : Obstacles(0, pos, 40) // Obstacles(int type, POINT pos, int size)
	{
		speed = 2;
        hBitAnim[0] = hBitAnim[1]= NULL;
        Width = Height = 0;
        setImage(hInstance);

        hitW = Width * 0.8;
        hitH = Height * 0.8;

        hitbox.left = pos.x - hitW / 2;
        hitbox.top = pos.y - hitH / 2;
        hitbox.right = pos.x + hitW / 2;
        hitbox.bottom = pos.y + hitH / 2;
	}

    ~OBS_Random() {
        for (int i = 0; i < 2; i++)
            DeleteObject(hBitAnim[i]);
    }

    float LengthPts(int x1, int y1, int x2, int y2)
    {
        return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
    }


    bool PointInRange(int px, int py, int x_min, int x_max, int y_min, int y_max) {
        if (px < x_max && px > x_min && py < y_max && py > y_min)
            return true;
        return false;
    }

    bool Check_PlayerCollision(RECT playerRect, float cameraY) {
        int screenY = (int)(pos.y - cameraY);

        POINT corners[4] = {
            {playerRect.left, playerRect.top},
            {playerRect.right, playerRect.top},
            {playerRect.left, playerRect.bottom},
            {playerRect.right, playerRect.bottom}
        };
    }




    void setImage(HINSTANCE hInstance) override {
        hBitAnim[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP88));
        hBitAnim[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP89));

        GetObject(hBitAnim[0], sizeof(BITMAP), &bmp);
        Width = bmp.bmWidth;
        Height = bmp.bmHeight;
    }

    void Update(float cameraDelta, float cameraY) override {
        hitbox.left = pos.x - hitW / 2 + 150;
        hitbox.top = (pos.y - cameraY) - hitH / 2 + 100;
        hitbox.right = hitbox.left + 130;
        hitbox.bottom = hitbox.top + 70;

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