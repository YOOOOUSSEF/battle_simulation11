#pragma once
#include"AlienArmy.h"
#include"EarthArmy.h"
#include"AlliedArmy.h"
#include"randGen.h"
#include"LinkedQueue.h"
#include"HEAL.h"
class Game
{
private:
	int N, ES, ET,EG,HU, AS, AM, AD, Prob;
	int SmallPowerE, HighPowerE;
	int SmallHealthE, HighHealthE;
	int SmallAttackCapE, HighAttackCapE;
	int SmallPowerA, HighPowerA;
	int SmallHealthA, HighHealthA;
	int SmallAttackCapA, HighAttackCapA;
	int SmallPowerS, HighPowerS;
	int SmallHealthS, HighHealthS;
	int SmallAttackCapS, HighAttackCapS;
	int infectionthreshold;
	EarthArmy earthArmy;
	AlienArmy alienArmy;
	AlliedArmy alliedarmy;
	randGen R;
	
	LinkedQueue<Unit*> KilledList;
	
	priQueue<Unit*>Uml1; //for earth soilder
	LinkedQueue<Unit*>Uml2; //for earth tank
	int killedCount,timeStep, num_of_healed;
	int killedcountEs, killedcountEt, killedcountEg, killedcountAs, killedcountAm, killedcountAd;
	int counterForUML1, counterForUML2;
	int mode;
	char fileName[100];
	int infectionprob;
	int infectioncount,Real_infection_count;
public:
	Game();
	bool AttackLogic();
	void HealLogic();
	void LoadFromFile(char filename[]);
	void addToKilledList(Unit* killed);
	void PrintKilledList() const; 
	void printQueues(LinkedQueue<Unit*>, LinkedQueue<Unit*>, LinkedQueue<Unit*>, LinkedQueue<Unit*>, LinkedQueue<Unit*>
	, LinkedQueue<Unit*>, LinkedQueue<Unit*>, LinkedQueue<Unit*>, LinkedQueue<Unit*>, LinkedQueue<Unit*>, LinkedQueue<Unit*>
		, LinkedQueue<Unit*>, LinkedQueue<Unit*>);
	void printUML();
	
	void CreateOutputFile(char filename[]);
	void infectionspread();
	bool needSU();
	~Game();
};

