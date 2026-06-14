#pragma once
#include <windows.h>
#include <random>
#include "resource.h"
#include "Obstacles.h"

class OBS_Path : public Obstacles {
private:
    HBITMAP hPath;      // 배열 대신 1개만 저장하도록 변경
    int pathType;       // 0, 1, 2 중 당첨된 랜덤 타입
    int winWidth;
    int PathWidth, PathHeight;
    int stepHeight = 1200; // 통로 y길이

    // 타입별 원본 충돌 좌표 데이터
    POINT Points_0L[2] = { {420, 0}, {420, 720} };
    POINT Points_0R[2] = { {795, 0}, {795, 720} };

    POINT Points_1L[2] = { {824, 0}, {179, 720} };
    POINT Points_1R[2] = { {1081, 0}, {436, 720} };

    POINT Points_3L[2] = { {132, 0}, {777, 720} };
    POINT Points_3R[2] = { {389, 0}, {1034, 720} };

    // 현재 선택된 경로의 포인터를 가리킬 변수
    POINT* LeftPts;
    POINT* RightPts;
    int PtsCount = 2; // 점의 개수 (모두 2개로 동일하므로 하나로 통일)

public:
    OBS_Path(POINT pos, HINSTANCE hInstance, int winWidth) : Obstacles(3, pos, 40)
    {
        this->winWidth = winWidth;
        hPath = NULL;
        Width = Height = 0;
        Height = stepHeight;

        // 1. 객체 생성 시 0~2 중 하나를 랜덤으로 선택
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 2);
        pathType = dist(gen);

        // 2. 선택된 타입에 맞게 충돌 좌표 포인터 매핑
        if (pathType == 0) {
            LeftPts = Points_0L;
            RightPts = Points_0R;
        }
        else if (pathType == 1) {
            LeftPts = Points_1L;
            RightPts = Points_1R;
        }
        else {
            LeftPts = Points_3L;
            RightPts = Points_3R;
        }

        setImage(hInstance);
    }

    ~OBS_Path() {
        if (hPath != NULL) {
            DeleteObject(hPath);
        }
    }

    void setImage(HINSTANCE hInstance) override {
        // 3. 선택된 타입의 비트맵만 메모리에 로드 (메모리 절약)
        int bmpID;
        if (pathType == 0) bmpID = IDB_BITMAP90;
        else if (pathType == 1) bmpID = IDB_BITMAP91;
        else bmpID = IDB_BITMAP93;

        hPath = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(bmpID));

        BITMAP tempBmp;
        GetObject(hPath, sizeof(BITMAP), &tempBmp);
        PathWidth = tempBmp.bmWidth;
        PathHeight = tempBmp.bmHeight;
    }

    void Update(float cameraDelta, float cameraY) override {
        hitbox.left = pos.x - size / 2;
        hitbox.top = (pos.y - cameraY) - size / 2;
        hitbox.right = pos.x + size / 2;
        hitbox.bottom = (pos.y - cameraY) + size / 2;
    }

    void Render(HDC mdc, float cameraY) override {
        int screenY = (int)(pos.y - cameraY);
        int segTop = screenY - 70; // 단일 객체이므로 for문의 인덱스 오프셋 제거

        HDC hMemDC = CreateCompatibleDC(mdc);
        HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hPath);

        // 선택된 이미지만 렌더링
        TransparentBlt(mdc, 0, segTop, winWidth, stepHeight,
            hMemDC, 0, 0, PathWidth, PathHeight, RGB(0, 255, 0));

        SelectObject(hMemDC, oldBit);
        DeleteDC(hMemDC);
    }

    // y좌표에 있는 (x1, y1)에서 (x2, y2)까지 함수
    float Getwall(float y, float x1, float y1, float x2, float y2)
    {
        if (y2 == y1) {
            return x1;
        }
        else {
            float t = (y - y1) / (y2 - y1); // 진행률 (0~1)
            return x1 + t * (x2 - x1);
        }
    }

    bool Check(RECT rect, float cameraY) override
    {
        int screenY = (int)(pos.y - cameraY);
        int Top = screenY - 70;
        int Bottom = Top + stepHeight;

        // 렉트가 이 통로의 y범위를 완전히 벗어나면 검사 패스
        if (rect.bottom < Top || rect.top > Bottom)
            return false;

        float scaleX = (float)winWidth / PathWidth;
        float scaleY = (float)stepHeight / PathHeight;

        // 화면 y -> 비트맵 원본 y로 변환
        float bitmapTop = (rect.top - Top) / scaleY;
        float bitmapBottom = (rect.bottom - Top) / scaleY;

        // 현재 선택된 통로(LeftPts, RightPts)에 대해서만 충돌 검사
        float LtopX = Getwall(bitmapTop, LeftPts[0].x, LeftPts[0].y, LeftPts[1].x, LeftPts[1].y);
        float LbottomX = Getwall(bitmapBottom, LeftPts[0].x, LeftPts[0].y, LeftPts[1].x, LeftPts[1].y);

        float RtopX = Getwall(bitmapTop, RightPts[0].x, RightPts[0].y, RightPts[1].x, RightPts[1].y);
        float RbottomX = Getwall(bitmapBottom, RightPts[0].x, RightPts[0].y, RightPts[1].x, RightPts[1].y);

        float Ltop = LtopX * scaleX;
        float Lbottom = LbottomX * scaleX;
        float Rtop = RtopX * scaleX;
        float Rbottom = RbottomX * scaleX;

        // 왼쪽 벽을 넘어갔는지
        if (rect.left < Ltop || rect.left < Lbottom)
            return true;

        // 오른쪽 벽을 넘어갔는지
        if (rect.right > Rtop || rect.right > Rbottom)
            return true;

        return false;
    }
};