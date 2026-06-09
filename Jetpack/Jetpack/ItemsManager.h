#pragma once
#include "Items.h"
#include <windows.h>
#include <random>


#define DELETE_DISTANCE 5000 // 플레이어보다 아래로 DELETE_DIST 이상 떨어진 아이템 제거
extern HINSTANCE g_hInst;

struct ItemNode {
	Items* item; // 아이템
	ItemNode* next;
	ItemNode* prev;

	ItemNode(Items* newItem) : item(newItem), next(nullptr), prev(nullptr) {}
};

class ItemsManager {
private:
	ItemNode* head;
	int playerY;
	unsigned char type;
	int offsetY = 100;
	RECT win;


	std::uniform_int_distribution<int> randomX;
	std::uniform_int_distribution<int> randoffset;

	std::default_random_engine dre{ std::random_device{}() };

public:
	// 게임 오버 등 모든 아이템 초기화 시 사용
	void Clear() {
		ItemNode* current = head;
		while (current != nullptr) {
			ItemNode* next = current->next;
			delete current->item;
			delete current;
			current = next;
		}
		head = nullptr;
	}



	ItemsManager() : head(nullptr) {}
	~ItemsManager() {
		Clear();
	};

	void setWin(RECT win) {
		this->win = win;
		randomX.param(std::uniform_int_distribution<int>::param_type(win.left, win.right));
		randoffset.param(std::uniform_int_distribution<int>::param_type(800, 1500));

	}

	void Add_Item(Items* newItem) {
		ItemNode* newNode = new ItemNode(newItem);
		newNode->next = head;
		newNode->prev = nullptr;

		if (head != nullptr) {
			head->prev = newNode;
		}
		head = newNode;
	}

	void Render_Items(HDC hdc, float cameraY) {
		ItemNode* current = head;
		while (current != nullptr) {
			current->item->Render(hdc, cameraY);
			current = current->next;
		}
	}

	void Update_Items(float cameraDelta, float cameraY) {
		ItemNode* current = head;
		while (current != nullptr) {
			current->item->Update(cameraDelta, cameraY);
			current = current->next;
		}
	}

	void Delete_Items(float playerY) {
		ItemNode* current = head;
		while (current != nullptr) {
			if (current->item->getPos().y > playerY + DELETE_DISTANCE) {

				ItemNode* topDelete = current;

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

				delete topDelete->item;
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

		Items* topItem = head->item;
		int topItemY = topItem->getPos().y;
		int topItemSize = topItem->getSize();
		//int tItemWidth = topItem->getWidth();
		int topItemHeight = topItem->getHeight();

		const int spawnDistance = 1000; // 장애물이 생성될 플레이어와의 최소 거리

		//randoffset.param(std::uniform_int_distribution<int>::param_type(100, 500));

		offsetY = randoffset(dre);
		int newY = topItemY - topItemSize - topItemHeight - offsetY;

		switch (type) {

		case 0:
		{

			if (topItemY - topItemSize - topItemHeight > playerY - spawnDistance)
			{

				Add_Item(new Items(POINT{ randomX(dre),newY }, g_hInst));

			}

			break;
		}
		case 1:											
		{
			if (topItemY - topItemSize - topItemHeight > playerY - spawnDistance)
			{
				Add_Item(new Items(POINT{ randomX(dre),newY }, g_hInst));
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
		ItemNode* current = head;
		while (current != nullptr) {
			if (Check_Collision(playerHitbox, current->item->getHitBox())) {
				return true; // 충돌O
			}
			current = current->next;
		}
		return false; // 충돌X
	}
};