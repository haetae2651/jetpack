#pragma once
#include "Obstacles.h"
#include "Player.h"
#include <windows.h>

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

	void Update_Obstacles() {
		ObsNode* current = head;
		while (current != nullptr) {
			current->obs->Update();
			current = current->next;
		}
	}


};