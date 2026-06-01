#pragma once
#include <windows.h>
#include "resource.h"
#include "Obstacles.h"

class OBS_Path : public Obstacles {
private:
    HBITMAP hBitmap;
    int frameTimer = 0;
    int winWidth;

public:
    OBS_Path(POINT pos, HINSTANCE hInstance, int winWidth) : Obstacles(3, pos, 40)
    {
        this->winWidth = winWidth;
        hBitmap = NULL;
        Width = Height = 0;
        setImage(hInstance);
    }

    ~OBS_Path() {
        DeleteObject(hBitmap);
    }

    void setImage(HINSTANCE hInstance) override {
        hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));

        BITMAP tempBmp;
        GetObject(hBitmap, sizeof(BITMAP), &tempBmp);
        Width = tempBmp.bmWidth;
        Height = tempBmp.bmHeight;
    }

    void Update(float cameraDelta, float cameraY) override {
        hitbox.left = pos.x - size / 2;
        hitbox.top = (pos.y - cameraY) - size / 2;
        hitbox.right = pos.x + size / 2;
        hitbox.bottom = (pos.y - cameraY) + size / 2;
    }

    void Render(HDC hdc, float cameraY) override {
        int screenY = (int)(pos.y - cameraY);

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hBitmap);

        TransparentBlt(hdc, 0, screenY - Height / 2, winWidth, Height,
            hMemDC, 0, 0, Width, Height, RGB(0, 255, 0));


        SelectObject(hMemDC, oldBit);
        DeleteDC(hMemDC);
    }
};