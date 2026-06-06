#pragma once
#include <windows.h>
#include "Obstacles.h"
#include "resource.h"

#define Frame 4

extern int screenWidth;
extern int screenHeight;

class OBS_LeftRight : public Obstacles
{
private:

    HBITMAP hBitAnim_R[4];
    HBITMAP hBitAnim_L[4];
    int animCount = 0;
    int frameTimer = 10;

    BITMAP bmp;

    int hitW = Width * 0.8;
    int hitH = Height * 0.8;

    bool move_R = false;

public:
    OBS_LeftRight(POINT pos, HINSTANCE hInstance) : Obstacles(1, pos, 20) // Obstacles(int type, POINT pos, int size)
    {
        for (int i = 0; i < Frame; ++i) {
            hBitAnim_R[i] = NULL;
            hBitAnim_L[i] = NULL;
        }
        Width = Height = 0;

        setImage(hInstance);
        size = 40;
        speed = 5; 

        int hitW = Width * 0.8;
        int hitH = Height * 0.8;

        hitbox.left = pos.x - hitW / 2;
        hitbox.top = pos.y - hitH / 2;
        hitbox.right = pos.x + hitW / 2;
        hitbox.bottom = pos.y + hitH / 2;
    }

    ~OBS_LeftRight() {
        for (int i = 0; i < Frame; i++) {
            DeleteObject(hBitAnim_R[i]);
            DeleteObject(hBitAnim_L[i]);
        }
    }

    void setImage(HINSTANCE hInstance) override {
        hBitAnim_R[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP73));
        hBitAnim_R[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP74));
        hBitAnim_R[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP75));
        hBitAnim_R[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP76));

        hBitAnim_L[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP77));
        hBitAnim_L[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP78));
        hBitAnim_L[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP79));
        hBitAnim_L[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP80));


        GetObject(hBitAnim_R[0], sizeof(BITMAP), &bmp);
        GetObject(hBitAnim_L[0], sizeof(BITMAP), &bmp);
        Width = bmp.bmWidth;
        Height = bmp.bmHeight;
    }

    void Update(float cameraDelta, float cameraY) override {
		
        if (move_R) {
			pos.x += speed; 
			if (pos.x + Width >= screenWidth) {
				move_R = false; 
			}
		}
		else {
			pos.x -= speed; \
            if (pos.x <= 0) {  
                move_R = true;
            }
		}

        hitbox.left = pos.x - 1 / Width;
        hitbox.top = (pos.y - cameraY) - 1 / Height;
        hitbox.right = pos.x + Width;
        hitbox.bottom = (pos.y - cameraY) + Height;

        frameTimer++;
        if (frameTimer >= 10) {
            frameTimer = 0;
            animCount++;
            animCount %= Frame;
        }
    }

    void Render(HDC hdc, float cameraY) override {
        int screenY = (int)(pos.y - cameraY);

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hBitAnim_R[animCount]);
        GetObject(hBitAnim_R[animCount], sizeof(BITMAP), &bmp);
        Width = bmp.bmWidth;
        Height = bmp.bmHeight;

		if (move_R) {
			TransparentBlt(hdc, pos.x - size / 2, screenY - size / 2, size + Width, size + Height,
				hMemDC, 0, 0, Width, Height, RGB(0, 255, 0));
		}
		else {
			SelectObject(hMemDC, hBitAnim_L[animCount]);
			TransparentBlt(hdc, pos.x - size / 2, screenY - size / 2, size + Width, size + Height,
				hMemDC, 0, 0, Width, Height, RGB(0, 255, 0));
		}

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