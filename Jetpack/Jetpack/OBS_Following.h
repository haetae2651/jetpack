#pragma once
#pragma once
#include <windows.h>
#include "Obstacles.h"
#include "resource.h"
#include <cmath>

#define Frame_H 4

extern int screenWidth;
extern int screenHeight;

class OBS_Following : public Obstacles
{
private:
    HBITMAP hBitAnim_R[4];
    HBITMAP hBitAnim_L[4];
    HBITMAP hStar;

    int animCount = 0;
    int frameTimer = 10;

    int hitW = 0;
    int hitH = 0;

    float x = 0.0f; // x방향 속도
    float y = 0.0f; // y방향 속도

public:
    OBS_Following(POINT pos, HINSTANCE hInstance, int playerX, int playerY) : Obstacles(1, pos, 40)
    {
        for (int i = 0; i < Frame_H; ++i) {
            hBitAnim_R[i] = NULL;
            hBitAnim_L[i] = NULL;
        }
        hStar = NULL;
        Width = Height = 0;

        setImage(hInstance);

        hitW = Width * 0.8;
        hitH = Height * 0.8;

        hitbox.left = pos.x - hitW / 2;
        hitbox.top = pos.y - hitH / 2;
        hitbox.right = pos.x + hitW / 2;
        hitbox.bottom = pos.y + hitH / 2;

        // 생성 시점 플레이어 방향으로 이동 벡터 계산
        float dirX = (float)(playerX - pos.x);
        float dirY = (float)(playerY - pos.y);

        // 벡터 정규화 (방향만 뽑기)
        float length = sqrt(dirX * dirX + dirY * dirY);
        if (length != 0) {
            dirX /= length;
            dirY /= length;
        }

        float homingSpeed = 8.0f; // 속도 (조절 가능)
        x = dirX * homingSpeed;
        y = dirY * homingSpeed;
    }

    ~OBS_Following() {
        for (int i = 0; i < Frame_H; i++) {
            DeleteObject(hBitAnim_R[i]);
            DeleteObject(hBitAnim_L[i]);
        }
        DeleteObject(hStar);
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

        hStar = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP100));

        BITMAP tempBmp;
        GetObject(hBitAnim_R[0], sizeof(BITMAP), &tempBmp);
        Width = tempBmp.bmWidth;
        Height = tempBmp.bmHeight;
    }

    void Update(float cameraDelta, float cameraY) override {
        // 생성 시 계산한 방향으로 직선 이동
        pos.x += x;
        pos.y += y;

        // 히트박스 갱신
        hitbox.left = pos.x - hitW / 2;
        hitbox.top = (pos.y - cameraY) - hitH / 2;
        hitbox.right = pos.x + hitW / 2;
        hitbox.bottom = (pos.y - cameraY) + hitH / 2;

        frameTimer++;
        if (frameTimer >= 10) {
            frameTimer = 0;
            animCount++;
            animCount %= Frame_H;
        }
    }

    bool CheckPlayerCollision(RECT playerRect, float cameraY) {
        if (playerRect.right <= hitbox.left || playerRect.left >= hitbox.right ||
            playerRect.bottom <= hitbox.top || playerRect.top >= hitbox.bottom) {
            return false;
        }
        return true;
    }

    void Render(HDC hdc, float cameraY) override {
        int screenY = (int)(pos.y - cameraY);
        int score = (int)((-cameraY + 172) / 10);

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP oldBit;

        if (score <= 600) {
            if (x > 0) {
                oldBit = (HBITMAP)SelectObject(hMemDC, hBitAnim_R[animCount]);
            }
            else {
                oldBit = (HBITMAP)SelectObject(hMemDC, hBitAnim_L[animCount]);
            }

            BITMAP tempBmp;
            GetObject(x > 0 ? hBitAnim_R[animCount] : hBitAnim_L[animCount], sizeof(BITMAP), &tempBmp);
            int w = tempBmp.bmWidth;
            int h = tempBmp.bmHeight;

            TransparentBlt(hdc, pos.x - size / 2, screenY - size / 2, size + w, size + h,
                hMemDC, 0, 0, w, h, RGB(0, 255, 0));
        }
        else {
            BITMAP tempBmp;
            GetObject(hStar, sizeof(BITMAP), &tempBmp);
            oldBit = (HBITMAP)SelectObject(hMemDC, hStar);

            int starDrawSize = 60;
            TransparentBlt(hdc, pos.x - starDrawSize / 2, screenY - starDrawSize / 2, starDrawSize, starDrawSize,
                hMemDC, 0, 0, tempBmp.bmWidth, tempBmp.bmHeight, RGB(0, 255, 0));

            hitbox.left = pos.x - starDrawSize / 2;
            hitbox.top = screenY - starDrawSize / 2;
            hitbox.right = pos.x + starDrawSize / 2;
            hitbox.bottom = screenY + starDrawSize / 2;
        }

        SelectObject(hMemDC, oldBit);
        DeleteDC(hMemDC);
    }

};