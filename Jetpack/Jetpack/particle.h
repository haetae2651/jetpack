#pragma once
#include <windows.h>
class Particles
{

private:
	
	int particlenum;
	bool gravity;
	int vecX, vecY;
	
public:

	Particles(int num, int vecX, int vecY, bool gravity)
	{
		particlenum = num;
		this->vecX = vecX;
		this->vecY = vecY;
		this->gravity = gravity;
		
	}

};


struct Particle 
{
	int vecX, vecY;
	int speed;
	HBITMAP hBitmap;
};

struct Node
{
	Particle* part;
	Node* next;
};
