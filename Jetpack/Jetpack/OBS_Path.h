#pragma once
#include <windows.h>
#include "resource.h"
#include "Obstacles.h"

class OBS_Path : public Obstacles {
private:
    HBITMAP hPath[4];
    int frameTimer = 0;
    int winWidth;
    int PathWidth[4], PathHeight[4];

public:
    OBS_Path(POINT pos, HINSTANCE hInstance, int winWidth) : Obstacles(3, pos, 40)
    {
        this->winWidth = winWidth;
        hPath[0], hPath[1], hPath[2], hPath[3] = NULL;
        Width = Height = 0;
        setImage(hInstance);
    }

    ~OBS_Path() {
        for (int i = 0; i < 4; i++)
            DeleteObject(hPath[i]);
    }

    void setImage(HINSTANCE hInstance) override {
        hPath[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
        hPath[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP52));
        hPath[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP53));
        hPath[3] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP54));


        BITMAP tempBmp;
        for (int i = 0; i < 4; i++) {
            GetObject(hPath[i], sizeof(BITMAP), &tempBmp);
            PathWidth[i] = tempBmp.bmWidth;
            PathHeight[i] = tempBmp.bmHeight;
        }
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
        HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hPath[0]);


        //잠시 꺼놨다 - 06.11
        /*
        for (int i = 0; i < 4; i++) {
            SelectObject(hMemDC, hPath[i]);
            TransparentBlt(hdc, 0, screenY - PathHeight[i] * i - 70, winWidth, PathHeight[i],
                hMemDC, 0, 0, PathWidth[i], PathHeight[i], RGB(0, 255, 0));
        }
        */

        SelectObject(hMemDC, oldBit);
        DeleteDC(hMemDC);
    }
};