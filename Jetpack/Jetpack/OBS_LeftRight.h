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
    HBITMAP hStar;
    int animCount = 0;
    int frameTimer = 10;

    int hitW = Width * 0.8;
    int hitH = Height * 0.8;

    bool move_R = false;

    float k{};
    float ampfig{};
    float kfig{};

    std::uniform_real_distribution<float> randamp;
    std::uniform_real_distribution<float> randk;

    std::default_random_engine dre{ std::random_device{}() };


public:
    OBS_LeftRight(POINT pos, HINSTANCE hInstance, int speed) : Obstacles(1, pos, 40) // Obstacles(int type, POINT pos, int size)
    {
        for (int i = 0; i < Frame; ++i) {
            hBitAnim_R[i] = NULL;
            hBitAnim_L[i] = NULL;
        }
        Width = Height = 0;

        setImage(hInstance);
        this->speed = speed;

        hitW = Width * 0.8;
        hitH = Height * 0.8;

        hitbox.left = pos.x - hitW / 2;
        hitbox.top = pos.y - hitH / 2;
        hitbox.right = pos.x + hitW / 2;
        hitbox.bottom = pos.y + hitH / 2;


        randamp.param(std::uniform_real_distribution<float>::param_type(3.0f, 8.0f));
        randk.param(std::uniform_real_distribution<float>::param_type(0.01f, 0.05f));
        ampfig = randamp(dre);
        kfig = randk(dre);

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

        hStar = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP100));

        GetObject(hBitAnim_R[0], sizeof(BITMAP), &bmp);
        GetObject(hBitAnim_L[0], sizeof(BITMAP), &bmp);
        GetObject(hStar, sizeof(BITMAP), &bmp);

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
			pos.x -= speed;
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

        
        if (1 > 0) {
            float amplitude = ampfig;

            pos.y +=  static_cast<int>(sin(k) * amplitude);

            k += kfig;
        }



    }

    void Render(HDC hdc, float cameraY) override {
        int screenY = (int)(pos.y - cameraY);
        int score = (-cameraY + 172) / 10;

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP oldBit = (HBITMAP)SelectObject(hMemDC, hBitAnim_R[animCount]);
        GetObject(hBitAnim_R[animCount], sizeof(BITMAP), &bmp);
        Width = bmp.bmWidth;
        Height = bmp.bmHeight;

        if (score <= 600) { // 밤 시작 부분 600
            if (move_R) {
                TransparentBlt(hdc, pos.x - size / 2, screenY - size / 2, size + Width, size + Height,
                    hMemDC, 0, 0, Width, Height, RGB(0, 255, 0));
            }
            else {
                SelectObject(hMemDC, hBitAnim_L[animCount]);
                TransparentBlt(hdc, pos.x - size / 2, screenY - size / 2, size + Width, size + Height,
                    hMemDC, 0, 0, Width, Height, RGB(0, 255, 0));
            }
        }
		else {
            GetObject(hStar, sizeof(BITMAP), &bmp);
            int starWidth = bmp.bmWidth;
            int starHeight = bmp.bmHeight;

            int starDrawSize = 60;
            SelectObject(hMemDC, hStar);
            TransparentBlt(hdc, pos.x - starDrawSize / 2, screenY - starDrawSize / 2, starDrawSize, starDrawSize,
                hMemDC, 0, 0, starWidth, starHeight, RGB(0, 255, 0));

            // 별 히트박스
            hitbox.left = pos.x - starDrawSize / 2;
            hitbox.top = screenY - starDrawSize / 2;
            hitbox.right = pos.x + starDrawSize / 2;
            hitbox.bottom = screenY + starDrawSize / 2;
		}
        SelectObject(hMemDC, oldBit);
        DeleteDC(hMemDC);


    }
};