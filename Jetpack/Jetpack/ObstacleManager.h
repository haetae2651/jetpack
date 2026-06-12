#pragma once
#include "Obstacles.h"
#include "Player.h"
#include "OBS_Random.h"
#include "OBS_Path.h"
#include "OBS_LeftRight.h"
#include <windows.h>
#include <random>


#define DELETE_DISTANCE 5000 // 플레이어보다 아래로 DELETE_DIST 이상 떨어진 장애물 제거
extern HINSTANCE g_hInst;

struct ObsNode {
	Obstacles* obs; // 장애물
	ObsNode* next;
	ObsNode* prev;

	ObsNode(Obstacles* newObs) : obs(newObs), next(nullptr), prev(nullptr) {}
};

class ObstacleManager {
private :
	ObsNode* head;
	int playerY;
	unsigned char type;
	int offsetY = 100;
	RECT win;


	std::uniform_int_distribution<int> randomX;
	std::uniform_int_distribution<int> randoffset;
	std::uniform_int_distribution<int> randnum;
	std::uniform_int_distribution<int> Yoffset;

	std::default_random_engine dre{ std::random_device{}() };

public :
    // 게임 오버 등 모든 장애물 초기화 시 사용
    void Clear() {
        ObsNode* current = head;
        while (current != nullptr) {
            ObsNode* next = current->next;
            delete current->obs;
            delete current;
            current = next;
        }
        head = nullptr;
    }
	


    ObstacleManager() : head(nullptr) {}
    ~ObstacleManager() {
        Clear();
    };

	void setWin(RECT win) {
		this->win = win; 
		randomX.param(std::uniform_int_distribution<int>::param_type(win.left + 100, win.right - 100));
		randoffset.param(std::uniform_int_distribution<int>::param_type(100, 500));
		randnum.param(std::uniform_int_distribution<int>::param_type(1, 4));
		Yoffset.param(std::uniform_int_distribution<int>::param_type(-100, 100));

	}

	void Add_Obstacle(Obstacles* newObs) {
		ObsNode* newNode = new ObsNode(newObs);
		newNode->next = head;
		newNode->prev = nullptr;

		if (head != nullptr) {
			head->prev = newNode;
		}
		head = newNode;
	}
    
	void Render_Obstacles(HDC hdc, float cameraY) {
		ObsNode* current = head;
		while (current != nullptr) {
			current->obs->Render(hdc, cameraY);
			current = current->next;
		}
	}

	void Update_Obstacles(float cameraDelta, float cameraY) {
		ObsNode* current = head;
		while (current != nullptr) {
			current->obs->Update(cameraDelta, cameraY);
			current = current->next;
		}
	}

	void Delete_Obstacles(float playerY) {
		ObsNode* current = head;
		while (current != nullptr) {
			if (current->obs->getPos().y > playerY + DELETE_DISTANCE) {
				
				ObsNode* topDelete = current;
				
				if (current->prev == nullptr) {
					head = current->next;
				}
				else {
					current->prev->next = current->next;

				}
				if (current->next != nullptr) {
					current->next->prev = current->prev;
				}	

				current = current->next;

				delete topDelete->obs;
				delete topDelete;
			}
			else {
				current = current->next;
			}
		}
	}


	void AutoAdd(int type, int playerY)
	{
		if (head == nullptr)
		{
			return;
		}

		Obstacles* topObs = head->obs;
		int topObsY = topObs->getPos().y;
		int topObsSize = topObs->getSize();
		//int tObsWidth = topObs->getWidth();
		int topObsHeight = topObs->getHeight();

		const int spawnDistance = 1000; // 장애물이 생성될 플레이어와의 최소 거리
		offsetY = randoffset(dre);
		int newY = topObsY - topObsSize - topObsHeight - offsetY;

		switch (type) {

		case 0:											// OBS_Random
		{
			
			if (topObsY - topObsSize - topObsHeight > playerY - spawnDistance)
			{
				int num = randnum(dre);
				for (int i = 0; i < num; ++i)
				{
					Add_Obstacle(new OBS_Random(POINT{ randomX(dre),newY + Yoffset(dre)}, g_hInst));

				}

			}

			break;
		}
		case 1:											// OBS_LeftRight
		{
			if (topObsY - topObsSize - topObsHeight > playerY - spawnDistance)
			{
				//나중에 플레이어Y에 따른 속도조정
				Add_Obstacle(new OBS_LeftRight(POINT{ randomX(dre),newY }, g_hInst, 5));		//OBS_LeftRight(POINT pos, HINSTANCE hInstance, int speed)
			}
			break;
		}
		case 2:											// OBS_Follow
		{
			if (topObsY - topObsSize - topObsHeight > playerY - spawnDistance)
			{
				//Add_Obstacle(new OBS_Follow(POINT{ randomX(dre),topObsY - topObsSize - topObsHeight - offsetY }, g_hInst)); // Follow.h 아직 못 만듦 
			}
			break;
		}
		case 3:											// OBS_Path
		{
			if (topObsY - topObsSize - topObsHeight > playerY - spawnDistance)
			{
				//Add_Obstacle(new OBS_Path(POINT{ 0,topObsY - topObsSize - topObsHeight - offsetY }, g_hInst, win.right));
			}
			break;

		}

		}
	}

	bool Check_Collision(const RECT& rect1, const RECT& rect2) {
		if (rect1.right <= rect2.left || rect1.left >= rect2.right ||
			rect1.bottom <= rect2.top || rect1.top >= rect2.bottom) {
			return false; // 충돌X
		}
		return true; // 충돌O
	}

	// 직접 메인에서 쓸 함수
	bool Check_PlayerCollision(const RECT& playerHitbox) {
		ObsNode* current = head;
		while (current != nullptr) {
			if (Check_Collision(playerHitbox, current->obs->getHitBox())) {
				return true; // 충돌O
			}
			current = current->next;
		}
		return false; // 충돌X
	}
};