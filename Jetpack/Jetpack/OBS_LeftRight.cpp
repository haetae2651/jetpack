#pragma once
#include <windows.h>
#include "Obstacles.h"
#include "resource.h"

#define Frame 16

class OBS_LeftRight : public Obstacles
{
private:

    HBITMAP hBitAnim[16];
    int animCount = 0;
    int frameTimer = 10;

    BITMAP bmp;

    int hitW = Width * 0.8;
    int hitH = Height * 0.8;

public:
    OBS_LeftRight(POINT pos, HINSTANCE hInstance) : Obstacles(2, pos, 20) // Obstacles(int type, POINT pos, int size)
    {
        for (int i = 0; i < Frame; ++i) {
            hBitAnim[i] = NULL;
        }
        Width = Height = 0;

        setImage(hInstance);
        size = 40;

        hitbox.left = pos.x - hitW / 2;
        hitbox.top = pos.y - hitH / 2;
        hitbox.right = pos.x + hitW / 2;
        hitbox.bottom = pos.y + hitH / 2;
    }

    ~OBS_LeftRight() {
        for (int i = 0; i < Frame; i++)
            DeleteObject(hBitAnim[i]);
    }

    void setImage(HINSTANCE hInstance) override {
        hBitAnim[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP73));
        hBitAnim[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP74));
        hBitAnim[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP75));
        hBitAnim[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP76));
        hBitAnim[4] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP77));
        hBitAnim[5] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP78));
        hBitAnim[6] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP79));
        hBitAnim[7] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP80));
        hBitAnim[8] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP81));
        hBitAnim[9] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP82));
        hBitAnim[10] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP83));
        hBitAnim[11] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP84));
        hBitAnim[12] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP85));
        hBitAnim[13] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP86));
        hBitAnim[14] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP87));
        hBitAnim[15] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP88));

        GetObject(hBitAnim[0], sizeof(BITMAP), &bmp);
        Width = bmp.bmWidth;
        Height = bmp.bmHeight;
    }

    void Update(float cameraDelta, float cameraY) override {
        hitbox.left = pos.x - 1 / Width + 30;
        hitbox.top = (pos.y - cameraY) - 1 / Height + 20;
        hitbox.right = pos.x + Width / 1.25;
        hitbox.bottom = (pos.y - cameraY) + Height / 1.25;

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