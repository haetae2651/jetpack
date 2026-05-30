#pragma once
#include "Obstacles.h"
#include "Player.h"
#include "OBS_Random.h"
#include <windows.h>
#include <random>

#define DELETE_DISTANCE 500 // 플레이어보다 아래로 DELETE_DIST 이상 떨어진 장애물 제거
extern HINSTANCE g_hInst;

struct ObsNode {
	Obstacles* obs;
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
		randomX.param(std::uniform_int_distribution<int>::param_type(win.left, win.right));
	
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

	void Update_Obstacles(float cameraDelta) {
		ObsNode* current = head;
		while (current != nullptr) {
			current->obs->Update(cameraDelta);
			current = current->next;
		}
	}

	void Delete_Obstacles(float playerY) {
		ObsNode* current = head;
		while (current != nullptr) {
			if (current->obs->getPos().y > playerY + DELETE_DISTANCE) {
				
				ObsNode* toDelete = current;
				
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

				delete toDelete->obs;
				delete toDelete;
			}
			else {
				current = current->next;
			}
		}
	}


	void AutoAdd(int type, int playerY)
	{
		Obstacles* topObs = head->obs;
		int tObsY = topObs->getPos().y;		
		int tObsSize = topObs->getSize();
		//int tObsWidth = topObs->getWidth();
		int tObsHeight = topObs->getHeight();

		switch (type) {

		case 0:											// OBS_Random
		{
			if (playerY > tObsY + tObsSize + tObsHeight)
			{

				Add_Obstacle(new OBS_Random(POINT{ randomX(dre),tObsY + tObsSize + tObsHeight + offsetY}, g_hInst));

			}



			break;
		}




		}
	}

};