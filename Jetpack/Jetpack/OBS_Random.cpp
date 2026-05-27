#pragma once
#include <windows.h>
#include "resource.h"
#include "Obstacles.h"
#include <string>

class OBS_Random : public Obstacles {
private:
    HBITMAP hBitAnim[3]; // 애니메이션 프레임 3개
    int animCount = 0;
    int chaWidth, chaHeight;
    int size = 40;
    int frameTimer = 10;

public :
	OBS_Random(POINT pos, HINSTANCE hInstance) : Obstacles(0, pos, 20) // Obstacles(int type, POINT pos, int size)
	{
		speed = 2;
        hBitAnim[0] = hBitAnim[1] = hBitAnim[2] = NULL;
        chaWidth = chaHeight = 0;
        setImage(hInstance);
	}

    ~OBS_Random() {
        for (int i = 0; i < 3; i++)
            DeleteObject(hBitAnim[i]);
    }

    void setImage(HINSTANCE hInstance) override {
        BITMAP bmp;
        hBitAnim[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
        hBitAnim[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
        hBitAnim[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));

        // 이미지 크기 가져오기
        GetObject(hBitAnim[0], sizeof(BITMAP), &bmp);
        chaWidth = bmp.bmWidth;
        chaHeight = bmp.bmHeight;
    }

    void Update(float cameraDelta) override {
        pos.y += speed + cameraDelta; 

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
        HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hBitAnim[animCount]); // 현재 프레임

        //TransparentBlt(hdc, pos.x - size / 2, screenY - size / 2, size, size,
        //    hMemDC, 0, 0, chaWidth, chaHeight, RGB(0, 255, 0)); // 초록색 제거

        BitBlt(
            hdc,
            pos.x - size / 2,
            screenY - size / 2,
            chaWidth,
            chaHeight,
            hMemDC,
            0, 0,
            SRCCOPY
        );

        SelectObject(hMemDC, oldBit);
        DeleteDC(hMemDC);
    }
};