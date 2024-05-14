#pragma once
#include"LinkedQueue.h""
#include"priQueue.h"
#include"Unit.h"
#include"ArrayStack.h"
class AlliedArmy
{
	LinkedQueue<Unit*> SU;
	int id;
	int SUcount;
public:
	AlliedArmy();
	void addSU(Unit* soldier);
	bool RemoveSU(Unit*& EarthSoldier);
	void printSU();
	int  getSUcount();
	bool PeekSu(Unit*&);
	~AlliedArmy();
};

