#pragma once
#include "Obstacles.h"
#include "Player.h"
#include <windows.h>

#define DELETE_DISTANCE 300 // 플레이어보다 아래로 DELETE_DIST 이상 떨어진 장애물 제거

struct ObsNode {
	Obstacles* obs;
	ObsNode* next;
};

class ObstacleManager {
private :
	ObsNode* head;

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

	void Add_Obstacle(Obstacles* newObs) {
		ObsNode* newNode = new ObsNode;
		newNode->obs = newObs;
		newNode->next = head;
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

	// 플레이어Y의 위치에 따라 장애물 제거하는 함수
	void Delete_Obstacles(float playerY) {
		ObsNode* current = head;
		ObsNode* prev = nullptr;
		while (current != nullptr) {
			if (current->obs->getPos().y > playerY + DELETE_DISTANCE) { // 플레이어보다 아래로 200 이상 떨어진 장애물 제거
				if (prev == nullptr) {
					head = current->next;
				}
				else {
					prev->next = current->next;
				}
				delete current->obs;
				delete current;
				current = (prev == nullptr) ? head : prev->next;
			}
			else {
				prev = current;
				current = current->next;
			}
		}
	}

};