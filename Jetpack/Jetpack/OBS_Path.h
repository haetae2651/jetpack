#pragma once
#include <windows.h>
#include "resource.h"
#include "Obstacles.h"

class OBS_Path : public Obstacles {
private:
    HBITMAP hPath[3];
    int frameTimer = 0;
    int winWidth;
    int PathWidth[3], PathHeight[3];
    int stepHeight = 1200; // 통로 y길이

    POINT Points_0L[2] = { {420, 0}, {420, 720} };
    POINT Points_0R[2] = { {795, 0}, {795, 720} };
    
    POINT Points_1L[2] = { {824, 0}, {179, 720} };
    POINT Points_1R[2] = { {1081, 0}, {436, 720} };
    
    POINT Points_3L[2] = { {132, 0}, {777, 720} };
    POINT Points_3R[2] = { {389, 0}, {1034, 720} };

    // hPath[i]에 대응하는 점배열 + 점 개수 묶음
    POINT* LeftPts[3] = { Points_0L, Points_1L, Points_3L };
    int    LeftCount[3] = { 2, 2, 2 };

    POINT* RightPts[3] = { Points_0R, Points_1R, Points_3R };
    int    RightCount[3] = { 2, 2, 2 };

public:
    OBS_Path(POINT pos, HINSTANCE hInstance, int winWidth) : Obstacles(3, pos, 40)
    {
        this->winWidth = winWidth;
        hPath[0] = NULL, hPath[1] = NULL, hPath[2] = NULL;
        Width = Height = 0;
        setImage(hInstance);
    }

    ~OBS_Path() {
        for (int i = 0; i < 3; i++)
            DeleteObject(hPath[i]);
    }

    void setImage(HINSTANCE hInstance) override {
        hPath[0] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP90));
        hPath[1] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP91));
        hPath[2] = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP93));


        BITMAP tempBmp;
        for (int i = 0; i < 3; i++) {
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

    void Render(HDC mdc, float cameraY) override {
        int screenY = (int)(pos.y - cameraY);

        HDC hMemDC = CreateCompatibleDC(mdc);
        HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hPath[0]);

        for (int i = 0; i < 3; i++) {
            SelectObject(hMemDC, hPath[i]);
            TransparentBlt(mdc, 0, screenY - stepHeight * i - 70, winWidth, stepHeight,
                hMemDC, 0, 0, PathWidth[i], PathHeight[i], RGB(0, 255, 0));
        }

        SelectObject(hMemDC, oldBit);
        DeleteDC(hMemDC);

        // ----- 디버그: 벽 선분 그려보기 -----
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        HPEN oldPen = (HPEN)SelectObject(mdc, hPen);

        for (int i = 0; i < 3; i++) {
            int segTop = screenY - stepHeight * i - 70;
            float scaleX = (float)winWidth / PathWidth[i];
            float scaleY = (float)stepHeight / PathHeight[i];

            // 왼쪽 벽 - 점들을 순서대로 이어서 그림
            for (int j = 0; j < LeftCount[i] - 1; j++) {
                int x1 = (int)(LeftPts[i][j].x * scaleX);
                int y1 = segTop + (int)(LeftPts[i][j].y * scaleY);
                int x2 = (int)(LeftPts[i][j + 1].x * scaleX);
                int y2 = segTop + (int)(LeftPts[i][j + 1].y * scaleY);
                MoveToEx(mdc, x1, y1, NULL);
                LineTo(mdc, x2, y2);
            }

            // 오른쪽 벽
            for (int j = 0; j < RightCount[i] - 1; j++) {
                int x1 = (int)(RightPts[i][j].x * scaleX);
                int y1 = segTop + (int)(RightPts[i][j].y * scaleY);
                int x2 = (int)(RightPts[i][j + 1].x * scaleX);
                int y2 = segTop + (int)(RightPts[i][j + 1].y * scaleY);
                MoveToEx(mdc, x1, y1, NULL);
                LineTo(mdc, x2, y2);

            }

            // 가로 - 귀찮으니 가로 한 줄로 그림
            for (int i = 0; i < 3; i++) {
                int segTop = screenY - stepHeight * i - 80;
                int segBottom = segTop + stepHeight;

                MoveToEx(mdc, 0, segTop, NULL);
                LineTo(mdc, winWidth, segTop);

                MoveToEx(mdc, 0, segBottom, NULL);
                LineTo(mdc, winWidth, segBottom);
            }
        }

        SelectObject(mdc, oldPen);
        DeleteObject(hPen);
        // -----------------------------------
    }

    // y좌표에 있는 (x1, y1)에서 (x2, y2)까지 함수
    float Getwall(float y, float x1, float y1, float x2, float y2)
    {
        if (y2 == y1) {
            return x1; 
        }
        else {
			float t = (y - y1) / (y2 - y1); // (진행률) 0~1사이 값 나옴, y가 y1과 y2 사이에 있을 때만 유효하다
            return x1 + t * (x2 - x1);
        }
    }

    bool Check(RECT rect, float cameraY) override
    {
        int screenY = (int)(pos.y - cameraY);



        for (int i = 0; i < 3; i++) {
            int Top = screenY - stepHeight * i - 70;
            int Bottom = Top + stepHeight;

            if (rect.bottom < Top || rect.top > Bottom)
                continue;



            float scaleX = (float)winWidth / PathWidth[i];
            float scaleY = (float)stepHeight / PathHeight[i];



            // 화면 y -> 비트맵 원본 y로 변환
            float bitmapTop = (rect.top - Top) / scaleY;
            float bitmapBottom = (rect.bottom - Top) / scaleY;

            float LtopX = Getwall(bitmapTop, 
                LeftPts[i][0].x, 
                LeftPts[i][0].y, 
                LeftPts[i][1].x, 
                LeftPts[i][1].y);
            float LbottomX = Getwall(bitmapBottom,
                LeftPts[i][0].x,
                LeftPts[i][0].y,
                LeftPts[i][1].x,
                LeftPts[i][1].y);
            float RtopX = Getwall(bitmapTop, 
                RightPts[i][0].x, 
                RightPts[i][0].y, 
                RightPts[i][1].x, 
                RightPts[i][1].y);
            float RbottomX = Getwall(bitmapBottom, 
                RightPts[i][0].x, 
                RightPts[i][0].y, 
                RightPts[i][1].x, 
                RightPts[i][1].y);

            float Ltop = LtopX * scaleX;
            float Lbottom = LbottomX * scaleX;
            float Rtop = RtopX * scaleX;
            float Rbottom = RbottomX * scaleX;



            if (rect.left < Ltop || rect.left < Lbottom)
                return true;




            if (rect.right > Rtop || rect.right > Rbottom)
                return true; // 트루면 충돌
        }

        return false;
    }
};