#include "Game.h"
#include <fstream>
#include<iostream>
#include<string.h>
//using namespace std;
Game::Game()
{
	int input=1;
 Real_infection_count=0;

	infectioncount = 0;
     mode = 0;
	num_of_healed = 0;
	counterForUML1 = counterForUML2 = 0;
	 timeStep= 0;
	 killedCount = 0;
	 killedcountEs = killedcountEt = killedcountEg = killedcountAs = killedcountAm = killedcountAd = 0;
	//intialize the data members of the game
	cout << "Enter the file Name" << endl;
	cin >> fileName;//entering the name of file
	cout << endl;
	LoadFromFile(fileName);//calling loading file function
	cout << "Enter the mode number" << endl;
	cout << "1- Interactive Mode" << endl << "2- Silent Mode" << endl ;
	cin >> mode;
	cout << endl;

	cout << "enter any Number except -1 to start " << endl;
	cin >> input;
	srand(time(NULL));
	bool flag = false;

	while (input!=-1&&(timeStep<=40||flag==false))
	{
		if(R.GenerateA(Prob)&&earthArmy.getId()<=999)//generate randomA from randGen
			for (int i = 0; i < N; i++) 
			{
				Unit* u = R.GenEarthUnit(ES,ET,EG,HU, SmallHealthE, HighHealthE, SmallPowerE, HighPowerE, SmallAttackCapE, HighAttackCapE);//generating an earth unit
				u->setTJ(timeStep);
				if (u->getTYPE() == 0)// if ES
					earthArmy.addEarthSoldier(u);//add to earth soldiers queue
				else if (u->getTYPE() == 1)//if ET
					earthArmy.addEarthTank(u);//add to earth Tanks stack
				else if (u->getTYPE() == 2) earthArmy.addEarthGunnery(u);//add to earth Gunnerys pri queue
				else {
					earthArmy.addHealUnit(u);
				}                          //add Heal Unit to HealList
			}
		

		if (R.GenerateA(Prob) && alienArmy.getId() <= 2999)//generate randomB for randGen
			for (int i = 0; i < N; i++) {
				Unit* u = R.GenAlienUnit(AS,AM , AD, SmallHealthA, HighHealthA, SmallPowerA, HighPowerA, SmallAttackCapA, HighAttackCapA);//generating an alien unit
				u->setTJ(timeStep);
				if (u->getTYPE() == 3)//if AS
					alienArmy.addAlienSoldier(u);//add to alien solders queue
				else if (u->getTYPE() == 4)//if AM
					alienArmy.addAlienMonster(u);//add to alien monsters array of pointers
				else alienArmy.addAlienDrone(u);//add to alien drones doubly linked queue of drones
			}
		if (needSU()&&alliedarmy.getId()<=4999)
		{
			if (R.GenerateA(Prob))//generate random c for randGen
				for (int i = 0; i < N; i++) {
					Unit* u = R.GenSU(SmallHealthS, HighHealthS, SmallPowerS, HighPowerS, SmallAttackCapS, HighAttackCapS);//generating an SU unit
					u->setTJ(timeStep);
					if (u->getTYPE() == 7)
						alliedarmy.addSU(u);//add to SU	
				}
		}
		flag=AttackLogic();

		timeStep++;
	
	}
	

}


void Game::LoadFromFile(char filename[]) {

	char filePath[100];
	ifstream file; // opening an input file
	int count = 0;
	while (filename[count] != '\0') {
		count++;
}
	for (int i = 0; i < count; i++)
		filePath[i] = filename[i];
	filePath[count] = '.';
	filePath[count+1] = 't';
	filePath[count+2] = 'x';
	filePath[count+3] = 't';
	filePath[count+4] = '\0';

	file.open(filePath, ios::in); // open the file with the concatenated file path
	if (!file.is_open()) {
		cerr << "Error opening file: " << filename << endl;
		return;
	}
	file >> N >> ES >> ET >> EG >>HU>> AS >> AM >> AD >> Prob;//assigning the loading parameters for generation of units
	file >> SmallPowerE >> HighPowerE >> SmallHealthE >>  HighHealthE >> SmallAttackCapE >> HighAttackCapE;//high a low value limits of every type units
	file >> SmallPowerA >> HighPowerA >> SmallHealthA >> HighHealthA >> SmallAttackCapA >> HighAttackCapA;
	file>> infectionprob;
	file >> infectionthreshold;
	file >> SmallPowerS >> HighPowerS >> SmallHealthS >> HighHealthS >> SmallAttackCapS >> HighAttackCapS;
	HighPowerE = -HighPowerE;//removing the minus sign of the high value
	HighPowerA = -HighPowerA;
	HighHealthE = -HighHealthE;
	HighHealthA = -HighHealthA;
	HighAttackCapA = -HighAttackCapA;
	HighAttackCapE = -HighAttackCapE;
	HighPowerS = -HighPowerS;
	HighHealthS = -HighHealthS;
	HighAttackCapS = -HighAttackCapS;
	file.close();
	
}

void Game::addToKilledList(Unit* killed)
{
	KilledList.enqueue(killed);
	killedCount++;
if(killed->getTYPE()==0)
	killedcountEs++;
if (killed->getTYPE() == 1)
	killedcountEt++;
if (killed->getTYPE() == 2)
	killedcountEg++;
if (killed->getTYPE() == 3)
	killedcountAs++;
if (killed->getTYPE() == 4)
	killedcountAm++;
if (killed->getTYPE() == 5)
	killedcountAd++;
}

void Game::PrintKilledList() const
{
	cout << killedCount << " Units [";
	KilledList.print_the_list();
	cout << "] ";
	cout << endl;
}







bool Game::AttackLogic() {
	
	LinkedQueue<Unit*> TempList;

	LinkedQueue<Unit*> AttackedFromSu;
	LinkedQueue<Unit*> AttackedFromAsforSu;
	LinkedQueue<Unit*> AttackedFromAmforSu;
	LinkedQueue<Unit*> AttackedFromES;
	LinkedQueue<Unit*> AttackedFromETforMonster;
	LinkedQueue<Unit*> AttackedFromETforSoilder;
	LinkedQueue<Unit*> AttackedFromEGforMonster;
	LinkedQueue<Unit*> AttackedFromEGforDrones;
	LinkedQueue<Unit*> AttackedFromASforEs;
	LinkedQueue<Unit*> AttackedFromAMforSoilder;
	LinkedQueue<Unit*> AttackedFromAMforTank;
	LinkedQueue<Unit*> AttackedFromADforGunnery;
	LinkedQueue<Unit*> AttackedFromADforTank;



	////////////////////////////////////////////////////////////Sever Unit
	Unit* Su = nullptr,*Als=nullptr;
	bool su;
	bool als;

	su = alliedarmy.PeekSu(Su);
	if (su) {
		AttackedFromSu.enqueue(Su);//the attacker unit
		for (int i = 0; i < Su->getAttackCap(); i++) {
			als = alienArmy.RemoveAlienSoldier(Als);
			if (als) {
				Su->Attack(Als);
				if (Als->getTa() == -1)
					Als->setTa(timeStep);
				if (Als->getHealth() <= 0) {
					Als->setTd(timeStep);
					addToKilledList(Als);
				}
				else
					TempList.enqueue(Als);

				AttackedFromSu.enqueue(Als);//List for printing the last attacked As
			}
		}
	}
	while (!TempList.isEmpty()) {
		Unit* unit = nullptr;
		TempList.dequeue(unit);
		alienArmy.addAlienSoldier(unit);
	}

	////////////////////////////////////////////////////////////Earth solider
	Unit* Es = nullptr, * As = nullptr;
	bool as = true, es = true;
	es = earthArmy.PeekEarthSoldier(Es);
	if(es)
	{
		if (!(Es->getinfection())) {

			AttackedFromES.enqueue(Es);//the attacker unit
			for (int i = 0; i < Es->getAttackCap(); i++) {
				as = alienArmy.RemoveAlienSoldier(As);
				if (as) {
					Es->Attack(As);
					if (As->getTa() == -1)
						As->setTa(timeStep);
					if (As->getHealth() <= 0) {
						As->setTd(timeStep);
						addToKilledList(As);
					}
					else
						TempList.enqueue(As);

					AttackedFromES.enqueue(As);//List for printing the last attacked As

				}
			}

		}
		else
		{

			if (es) {
				bool esnotin;
				Unit* EsNotIn = nullptr;
				AttackedFromES.enqueue(Es);//the attacker unit
				for (int i = 0; i < Es->getAttackCap(); i++) {
					esnotin = earthArmy.RemoveEarthSoldier(EsNotIn); //attack his friend
					if (esnotin) {
						Es->Attack(EsNotIn);
						if (EsNotIn->getTa() == -1)
							EsNotIn->setTa(timeStep);
						if (EsNotIn->getHealth() <= 0) {
							EsNotIn->setTd(timeStep);
							if (EsNotIn->getinfection()) {
								Real_infection_count--;
								infectioncount--;
							}
							addToKilledList(EsNotIn);
						}
						else
							TempList.enqueue(EsNotIn);

						AttackedFromES.enqueue(EsNotIn);//List for printing the last attacked As

					}
				}
			
			}
		}
	}
	
	while (!TempList.isEmpty()) {
		Unit* unit = nullptr;
		TempList.dequeue(unit);
		if (unit->getTYPE() == 3)
			alienArmy.addAlienSoldier(unit);
		else  earthArmy.addEarthSoldier(unit);
	}
	//////////////////////////////////////////////////////////tank
	Unit* Et = nullptr, * Am = nullptr;
	As = nullptr;
	bool et = true, am = true;
	et = earthArmy.PeekEarthTank(Et);
	if (et) {
		
		AttackedFromETforMonster.enqueue(Et);//the Attacker unit
		for (int i = 0; i < Et->getAttackCap(); ) {
			bool flag3 = false;
			am = alienArmy.RemoveAlienMonster(Am);
			if (am) {
				i++;
				flag3 = true;
				Et->Attack(Am);
				if (Am->getTa() == -1)
					Am->setTa(timeStep);
				if (Am->getHealth() <= 0) {
					Am->setTd(timeStep);
					addToKilledList(Am);
				}
				else
					TempList.enqueue(Am);
				
				AttackedFromETforMonster.enqueue(Am);//List for printing the last attacked Am by ET
			}

			if (earthArmy.getEScount() < .3 * alienArmy.getAScount()) {
				for (; i < Et->getAttackCap(); ) {
					bool flag1 = false, flag2 = false;
					as = alienArmy.RemoveAlienSoldier(As);
					if (as) {
						i++;
						flag1 = true;
						Et->Attack(As);
						if (As->getTa() == -1)
							As->setTa(timeStep);
						if (As->getHealth() <= 0) {
							As->setTd(timeStep);
							addToKilledList(As);
						}
						else
							TempList.enqueue(As);
						
						AttackedFromETforSoilder.enqueue(As);//List for printing the last attacked As
					}

					if (earthArmy.getEScount() >= .8 * alienArmy.getAScount())
						break;
					if (i < Et->getAttackCap()) {
						am = alienArmy.RemoveAlienMonster(Am);
						if (am) {
							i++;
							flag2 = true;
							Et->Attack(Am);
							if (Am->getTa() == -1)
								Am->setTa(timeStep);
							if (Am->getHealth() <= 0) {
								Am->setTd(timeStep);
								addToKilledList(Am);
							}
							else
								TempList.enqueue(Am);
							
							AttackedFromETforMonster.enqueue(Am);//List for printing the last attacked Am by ET
						}

					}
					if (!flag1 && !flag2)
						break;

				}
			}
			if (!flag3)
				break;
		}
	
	}
	while (!TempList.isEmpty()) {
		Unit* unit = nullptr;
		TempList.dequeue(unit);
		if (unit->getTYPE() == 3)
			alienArmy.addAlienSoldier(unit);
		else 
			alienArmy.addAlienMonster(unit);
	}
	///////////////////////////////////////////////////////////////Gunnery
	Unit* Eg = nullptr, * Adf = nullptr, * Adl = nullptr;
	Am = nullptr;
	bool eg = true, adf = true, adl = true;
	am = true;
	eg = earthArmy.PeekEarthGunnery(Eg);
	if (eg) {
		
		AttackedFromEGforMonster.enqueue(Eg);//the Attacker unit
		for (int i = 0; i < Eg->getAttackCap();) {
			bool flag1 = false, flag2 = false;
			am = alienArmy.RemoveAlienMonster(Am);
			if (am) {
				i++;
				flag1 = true;
				Eg->Attack(Am);
				if (Am->getTa() == -1)
					Am->setTa(timeStep);
				if (Am->getHealth() <= 0) {
					Am->setTd(timeStep);
					addToKilledList(Am);
				}
				else
					TempList.enqueue(Am);
				
				AttackedFromEGforMonster.enqueue(Am);//List for printing the last attacked Am by EG
			}

			if (i < Eg->getAttackCap()) {
				adl = alienArmy.RemoveAlienDroneLast(Adl);
			
					
					if (adl) {
						flag2 = true;
						i++;
						Eg->Attack(Adl);
						if (Adl->getTa() == -1)
							Adl->setTa(timeStep);
						if (Adl->getHealth() <= 0) {
							Adl->setTd(timeStep);
							addToKilledList(Adl);
						}
						else
							TempList.enqueue(Adl);
						
						AttackedFromEGforDrones.enqueue(Adl);//List for printing the last attacked AD
					}
					if (i < Eg->getAttackCap()) {
						adf = alienArmy.RemoveAlienDroneFirst(Adf);
						if (adf) {
							flag2 = true;
							i++;
							Eg->Attack(Adf);

							if (Adf->getTa() == -1)
								Adf->setTa(timeStep);

							if (Adf->getHealth() <= 0) {
								Adf->setTd(timeStep);
								addToKilledList(Adf);
							}
							else
								TempList.enqueue(Adf);

							AttackedFromEGforDrones.enqueue(Adf);//List for printing the last attacked AD
						}
					}
				
				
			}
			if (!flag1 && !flag2)
				break;

		}
	}
	int j = 0;
	while (!TempList.isEmpty()) {
		Unit* unit = nullptr;
		TempList.dequeue(unit);
		if (unit->getTYPE() == 4)
			alienArmy.addAlienMonster(unit);
		else if(unit->getTYPE()==5) {
			if (j % 2 == 0)
				alienArmy.addAlienDrone(unit);
			else
				alienArmy.addAlienDroneFirst(unit);
			j++;
		}
	}
	//////////////////////////////////////////////////////////aliensolider
	{
		Unit* Su = nullptr; bool su;
		Unit* Es = nullptr, * As = nullptr;
		bool as = true, es = true;
		as = alienArmy.PeekAlienSoldier(As);
		if (as)
		{
			
				AttackedFromASforEs.enqueue(As);//the Attacker unit
			for (int i = 0; i < As->getAttackCap(); )
			{
				bool flag1 = false, flag2 = false;
				su = alliedarmy.RemoveSU(Su);
				if (su) {
					i++;
					flag1 = true;
					As->Attack(Su);
					if (Su->getTa() == -1)
						Su->setTa(timeStep);
					if (Su->getHealth() <= 0) {
						Su->setTd(timeStep);
						addToKilledList(Su);
					}
					else
						TempList.enqueue(Su);
					AttackedFromAsforSu.enqueue(Su);
				}
				if (i < As->getAttackCap()) {
					es = earthArmy.RemoveEarthSoldier(Es);
					if (es)
					{
						i++;
						flag2 = true;
						As->Attack(Es);
						if (Es->getTa() == -1)Es->setTa(timeStep);
						if (Es->getHealth() <= 0)
						{
							if (Es->getTd() == -1)Es->setTd(timeStep);
							if (Es->getinfection()) {
								Real_infection_count--;
								infectioncount--;
							}
							addToKilledList(Es);
						}
						else if (Es->getHealth() < .2 * Es->getinitialhealth()) {
							Es->settimeUml(timeStep);
							Uml1.enqueue(Es, 10000 - Es->getHealth());
							counterForUML1++;
						}
						else
						{
							TempList.enqueue(Es);
						}

						AttackedFromASforEs.enqueue(Es);//List for printing the last attacked ES by AS
					}
				}
				if (!flag1 && !flag2)
					break;
			}
		
		}
	}
	while (!TempList.isEmpty()) {
		Unit* unit = nullptr;
		TempList.dequeue(unit);
		if (unit->getTYPE() == 0)
			earthArmy.addEarthSoldier(unit);
		else
			alliedarmy.addSU(unit);
	}
	///////////////////////////////////////////////////////////////////monster
	{

		Unit* Am = nullptr, * Es = nullptr, * Et = nullptr;
		bool am = false, es = false, et = false;

		am = alienArmy.PeekAlienMonster(Am);
		if (am)
		{
			
			AttackedFromAMforSoilder.enqueue(Am);//the Attacker unit

			for (int i = 0; i < Am->getAttackCap(); )
			{
				bool flag1 = false, flag2 = false,flag3=false;
				su = alliedarmy.RemoveSU(Su);
				if (su) {
					i++;
					flag3 = true;
					Am->Attack(Su);
					if (Su->getTa() == -1)
						Su->setTa(timeStep);
					if (Su->getHealth() <= 0) {
						Su->setTd(timeStep);
						addToKilledList(Su);
					}
					else
						TempList.enqueue(Su);
					AttackedFromAmforSu.enqueue(Su);
				}
				if (i < Am->getAttackCap()) {
					es = earthArmy.RemoveEarthSoldier(Es);
					if (es) {
						srand(time(NULL));
						int randinfection = rand() % 100 + 1;
						if (randinfection <= infectionprob)
						{
							if (!(Es->getimmunity())&&!Es->getinfection())
							{
								Es->setinfection(true);
								infectioncount++;
								Real_infection_count++;

							}
							
						}
						i++;
						flag1 = true;
						Am->Attack(Es);
						if (Es->getTa() == -1)Es->setTa(timeStep);
						if (Es->getHealth() <= 0)
						{
							Es->setTd(timeStep);
							if (Es->getinfection()) {
								Real_infection_count--;
								infectioncount--;
								
							}
							addToKilledList(Es);
						}
						else if (Es->getHealth() < .2 * Es->getinitialhealth()) {
							Es->settimeUml(timeStep);
							Uml1.enqueue(Es, 10000 - Es->getHealth());
							counterForUML1++;
						}
						else
						{
							TempList.enqueue(Es);
						}

						AttackedFromAMforSoilder.enqueue(Es);//List for printing the last attacked ES by AM
					}
				}
				if (i < Am->getAttackCap()) {
					et = earthArmy.RemoveEarthTank(Et);
					if (et) {
						i++;
						flag2 = true;
						Am->Attack(Et);
						if (Et->getTa() == -1)Et->setTa(timeStep);
						if (Et->getHealth() <= 0)
						{
							if (Et->getTd() == -1)Et->setTd(timeStep);
							addToKilledList(Et);
						}
						else if (Et->getHealth() < .2 * Et->getinitialhealth()) {
							Et->settimeUml(timeStep);
							Uml2.enqueue(Et);
							counterForUML2++;
						}
						else
						{
							TempList.enqueue(Et);
						}
						
							AttackedFromAMforTank.enqueue(Et);//List for printing the last attacked ET by AM
					}
				}

				if (!flag1 && !flag2&&!flag3)
					break;

			}
			
		}
	}
	while (!TempList.isEmpty()) {
		Unit* unit = nullptr;
		TempList.dequeue(unit);
	

		if (unit->getTYPE() == 0)
			earthArmy.addEarthSoldier(unit);
		else if (unit->getTYPE() == 1)
			earthArmy.addEarthTank(unit);
		else
			alliedarmy.addSU(unit);
	}

	{
		int capacity;
		///////////////////////////////////////////////////////////////////////////Drones
		Unit* Ad1 = nullptr, * Ad2 = nullptr, * Eg = nullptr, * Et = nullptr;
		bool ad1 = true, ad2 = true, eg = true, et = true;

		ad1 = alienArmy.PeekAlienDroneFirst(Ad1);
		ad2 = alienArmy.PeekAlienDroneLast(Ad2);
		if (ad1 && ad2)
		{
			
				AttackedFromADforGunnery.enqueue(Ad1);//the attacker units
				AttackedFromADforGunnery.enqueue(Ad2);//the attacker units
			
		if (Ad1->getAttackCap() < Ad2->getAttackCap()) capacity = Ad1->getAttackCap();
		else capacity = Ad2->getAttackCap();
	
			for (int i = 0; i < capacity; )
			{
				bool flag1 = false, flag2 = false;
				eg = earthArmy.RemoveEarthGunnery(Eg);

				if (eg) {
					i++;
					flag1 = true;
					Ad1->Attack(Eg);
					Ad2->Attack(Eg);
					if (Eg->getTa() == -1)  Eg->setTa(timeStep);
					if (Eg->getHealth() <= 0)
					{
						if (Eg->getTd() == -1)Eg->setTd(timeStep);
						addToKilledList(Eg);
					}
					else
					{
						TempList.enqueue(Eg);
					}
					
					AttackedFromADforGunnery.enqueue(Eg);//List for printing the last attacked EG by AD
				}
				if (i < capacity) {
					et = earthArmy.RemoveEarthTank(Et);
					if (et) {
						i++;
						flag2 = true;
						Ad1->Attack(Et);
						Ad2->Attack(Et);
						if (Et->getTa() == -1)Et->setTa(timeStep);
						if (Et->getHealth() <= 0)
						{
							if (Et->getTd() == -1)Et->setTd(timeStep);
							addToKilledList(Et);
						}
						else if (Et->getHealth() < .2 * Et->getinitialhealth()) {
							Et->settimeUml(timeStep);
							Uml2.enqueue(Et);
							counterForUML2++;
						}
						else
						{
							TempList.enqueue(Et);
						}
						
							AttackedFromADforTank.enqueue(Et);//List for printing the last attacked Et by AD
					}
				}
				if (!flag1 && !flag2)
					break;

			}
		
		}

	}
	while (!TempList.isEmpty()) {
		Unit* unit = nullptr;
		TempList.dequeue(unit);

		if (unit->getTYPE() == 1)
			earthArmy.addEarthTank(unit);
		else 
			earthArmy.addEarthGunnery(unit);
	}

	//////////////////////////////////////////////////////////
	infectionspread();

	/////////////////////////////////////////////////////////////

	HealLogic();                  // to heal the atttacked of Es && Et


	//////////////////////////////////////////////////////////
	bool flag = false;
	int earthcount = earthArmy.getEScount() + earthArmy.getEGcount() + earthArmy.getETcount()+counterForUML1+counterForUML2;
	int aliencount = alienArmy.getADcount() + alienArmy.getAMcount() + alienArmy.getAScount();
	if (earthcount == 0 && aliencount == 0)
		flag = true;
	else if (earthcount == 0)
		flag = true;
	else if (aliencount == 0)
		flag = true;
	else if (earthArmy.getEGcount() != 0 && alienArmy.getAScount() != 0 && (earthcount - earthArmy.getEGcount()) == 0 &&
		(aliencount - alienArmy.getAScount()) == 0)
		flag = true;
	else if (earthArmy.getEScount() != 0&&counterForUML1!=0 && alienArmy.getADcount() != 0 && (earthcount - earthArmy.getEScount()-counterForUML1) == 0 &&
		(aliencount - alienArmy.getADcount()) == 0)
		flag = true;
	else
		flag = false;

	char f[] = "Weak_Earth Vs Moderate_Alien";
	if (mode == 1) {
		printQueues(AttackedFromES, AttackedFromETforMonster, AttackedFromETforSoilder, AttackedFromEGforMonster, AttackedFromEGforDrones
			, AttackedFromASforEs, AttackedFromAMforSoilder, AttackedFromAMforTank, AttackedFromADforGunnery, AttackedFromADforTank
			, AttackedFromSu, AttackedFromAsforSu, AttackedFromAmforSu);
	}

	if (timeStep >= 40 && flag == true)
        CreateOutputFile(f);
	

	return flag;
}

void Game::HealLogic() {
	LinkedQueue<Unit*>TempListforsoilder;
	LinkedQueue<Unit*>TempListfortank;
	priQueue<Unit*>holderForuml1;
	LinkedQueue<Unit*>holderForuml2;
	int f;
	Unit* healunit = nullptr,*healed=nullptr;
	
	if(!Uml1.isEmpty()||!Uml2.isEmpty())
	if (earthArmy.RemoveHealUnit(healunit)) 
		for (int i = 0; i < healunit->getAttackCap(); i++) {
			if (Uml1.dequeue(healed, f)) {
				counterForUML1--;
				if (timeStep - healed->gettimeUml() > 10) {
					healed->setTd(timeStep);
					if (healed->getinfection()) {
						Real_infection_count--;
						infectioncount--;

					}
					addToKilledList(healed);
					i--;
					continue;
				}
				if (healed->getinfection())
					Real_infection_count--;
				healunit->Attack(healed);
				num_of_healed++;
				if (healed->getHealth() > .2 * healed->getinitialhealth()) {
					earthArmy.addEarthSoldier(healed);
					healed->settimeUml(-1);
				}
				else {
					TempListforsoilder.enqueue(healed);
					healed->settimeUml(timeStep);
				}

			}
			else if (Uml2.dequeue(healed)) {
				counterForUML2--;
				if (timeStep - healed->gettimeUml() > 10) {
					healed->setTd(timeStep);
					addToKilledList(healed);
					i--;
					continue;
				}
				healunit->Attack(healed);
				num_of_healed++;
				if (healed->getHealth() > .2 * healed->getinitialhealth()) {
					earthArmy.addEarthTank(healed);
					healed->settimeUml(-1);
				}
				else {
					TempListfortank.enqueue(healed);
					healed->settimeUml(timeStep);
				}
			}
			else
				break;
		}
	if (healunit) {
		delete healunit;
		healunit = nullptr;
	}
	///////////////////////////////////////////////////delete who exceeds 10 timestep
	Unit* rescuer = nullptr;
	int g;
	while (Uml1.dequeue(rescuer, g)) {
		if (timeStep - rescuer->gettimeUml() > 10) {
			rescuer->setTd(timeStep);
			if (rescuer->getinfection()) {
				Real_infection_count--;
				infectioncount--;

			}
			counterForUML1--;
			addToKilledList(rescuer);
		}
		else {
			holderForuml1.enqueue(rescuer, g);
		}
	}
	while (holderForuml1.dequeue(rescuer, g))
		Uml1.enqueue(rescuer, g);
	////////////////////////////////////////////////////delete who exceeds 10 timestep
	while (Uml2.dequeue(rescuer)) {
		if (timeStep - rescuer->gettimeUml() > 10) {
			rescuer->setTd(timeStep);
		
			counterForUML2--;
			addToKilledList(rescuer);
		}
		else {
			holderForuml2.enqueue(rescuer);
		}
	}
	while (holderForuml2.dequeue(rescuer))
		Uml2.enqueue(rescuer);
	//////////////////////////////////////////////////////////
	Unit* deleter = nullptr;
	while (TempListfortank.dequeue(deleter)) {
		Uml2.enqueue(deleter);
		counterForUML2++;
	}
	
	while (TempListforsoilder.dequeue(deleter)) {
		Uml1.enqueue(deleter, 10000 - deleter->getHealth());
		counterForUML1++;
	}
	
}











void Game::printQueues(LinkedQueue<Unit*>AfromEs, LinkedQueue<Unit*>AfromEtforAm, LinkedQueue<Unit*>AfromEtforAs, LinkedQueue<Unit*>AfromEgforAm,
	LinkedQueue<Unit*>AfromEgforAd, LinkedQueue<Unit*>AfromAs, LinkedQueue<Unit*>AfromAmforEs, LinkedQueue<Unit*>AfromAmforEt
	, LinkedQueue<Unit*>AfromAdforEg, LinkedQueue<Unit*>AfromAdforEt,  LinkedQueue<Unit*>AfromSu,  LinkedQueue<Unit*>AfromAsforSu
	, LinkedQueue<Unit*>AfromAmforSu){

	
	cout << "Current Timestep " << timeStep << endl;//cout current time Step
	cout << "============== Earth Army Alive Units ============== " << endl;//printing earth army lists
	earthArmy.printEarthSoldiers();
	earthArmy.printEarthTanks();
	earthArmy.printEarthGunnerys();
	cout << endl;
	cout << "============== Alien Army Alive Units ============== " << endl;//printing alien army lists
	alienArmy.printAlienSoldiers();
	alienArmy.printAlienDrones();
	alienArmy.printAlienMonsters();
	cout << endl;
	cout << "================= Allied Army Alive Units ===============" << endl;
	alliedarmy.printSU();
	cout << endl;
	
	cout << endl<<"==================== HealList ====================="<<endl;
	earthArmy.printHealUnits();
	cout <<endl;

	cout << "====================== Unit Maintenance List ===============" << endl;
	printUML();
	cout << endl;
	cout  << "================ infection percentage ====================" << endl;
	int earthSunits = earthArmy.getEScount() + counterForUML1;
	if (earthSunits != 0)
		cout << ((double)Real_infection_count / earthSunits)*100 << " %" << endl;
	else
		cout << "Not defined" << endl;
	cout << endl;

	cout << "===================== Units fighting at current step ===============" << endl;
	//Attacked from sever units
	Unit* Su = nullptr, * Als = nullptr;
	if (AfromSu.dequeue(Su)) {
		cout << "SU " << Su->getID() << " shots ";
		cout << "[";
		AfromSu.print_the_list();
		cout << "]"<<endl;
	}
//Attacked from earth soilder
	Unit* Es = nullptr, * Et = nullptr, * Eg = nullptr, * As = nullptr, * Am = nullptr,*Ad1=nullptr,*Ad2=nullptr;
	if (AfromEs.dequeue(Es)) {
		cout << "ES " << Es->getID() << " shots ";
		cout << "[";
		AfromEs.print_the_list();
		cout << "]" << endl;
	}
	//Attacked from earth tank
	if (AfromEtforAm.dequeue(Et)) {
		cout << "ET " << Et->getID() << " shots ";
		cout << "[";
		AfromEtforAm.print_the_list();
		cout << "] && [";
		AfromEtforAs.print_the_list();
		cout << "]" << endl;
	}
	//Attacked from earth Gunnery
	if (AfromEgforAm.dequeue(Eg)) {
		cout << "EG " << Eg->getID() << " shots ";
		cout << "[";
		AfromEgforAm.print_the_list();
		cout << "] && [";
		AfromEgforAd.print_the_list();
		cout << "]" << endl;
	}
	//Attacked from alien soilder
	if (AfromAs.dequeue(As)) {
		cout << "AS " << As->getID() << " shots ";
		cout << "[";
		AfromAs.print_the_list();
		cout << "]&&[";
		AfromAsforSu.print_the_list();
		cout << "]" << endl;
	}
	//Attacked from alien monster
	if (AfromAmforEs.dequeue(Am)) {
		cout << "AM " << Am->getID() << " shots ";
		cout << "[";
		AfromAmforEs.print_the_list();
		cout << "] && [";
		AfromAmforEt.print_the_list();
		cout << "]&&[";
		AfromAmforSu.print_the_list();
		cout << "]" << endl;
	}
	//Attacked from alien drones
	if (AfromAdforEg.dequeue(Ad1)&& AfromAdforEg.dequeue(Ad2)) {
		cout << "AD " << Ad1->getID()<<" && "<< Ad2->getID()<< " shots ";
		cout << "[";
		AfromAdforEg.print_the_list();
		cout << "] && [";
		AfromAdforEt.print_the_list();
		cout << "]" << endl;
	}
	cout << endl;
	cout << "============== Killed/Destructed Units ============= " << endl;
	PrintKilledList();//printing the killed list of units
	cout << endl;
}

void Game::printUML() {
	cout << counterForUML1 << " ES [";
	Uml1.print_the_list();
	cout << "]"<<endl;
	cout << counterForUML2 << " ET [";
	Uml2.print_the_list();
	cout << "]" << endl;
}

void Game::CreateOutputFile(char filename[]) {
	char filePath[100];
	ofstream file;
	int count = 0;
	while (filename[count] != '\0') {
		count++;
	}
	for (int i = 0; i < count; i++)
		filePath[i] = filename[i];
	filePath[count] = '.';
	filePath[count + 1] = 't';
	filePath[count + 2] = 'x';
	filePath[count + 3] = 't';
	filePath[count+ 4] = '\0';

	file.open(filePath, ios::out);

	Unit* forascending = nullptr;

	LinkedQueue<Unit*> temp;//i make it to return the values to killed List
	while (KilledList.dequeue(forascending)) {
		
		temp.enqueue(forascending);
	}
	

	
	int Df, Dd, Db;
	int Total_Df_E = 0, Total_Dd_E = 0, Total_Db_E = 0, counter_E_for_Ave = 0;
	int Total_Df_A = 0, Total_Dd_A = 0, Total_Db_A = 0, counter_A_for_Ave = 0;
	while (temp.dequeue(forascending)) {//printing the Killed List  ascendingly by Td
		KilledList.enqueue(forascending);
		file << "Td  ID  Tj  Df  Dd  Db" << endl;
		file << forascending->getTd();
		if (forascending->getTd() > 9)
			file << "  ";
		else
			file << "   ";

		file << forascending->getID();
		if (forascending->getID() > 99)
			file << " ";
		else if (forascending->getID() > 9)
			file << "  ";
		else
			file << "   ";

		file << forascending->getTj();
		if (forascending->getTj() > 9)
			file << "  ";
		else
			file << "   ";

		Df = forascending->getTa() - forascending->getTj();
		file << Df;
		if (Df > 9)
			file << "  ";
		else
			file << "   ";

		Dd = forascending->getTd() - forascending->getTa();
		file << Dd;
		if (Dd > 9)
			file << "  ";
		else
			file << "   ";

		Db = forascending->getTd() - forascending->getTj();
		file << Db;
		if (Db > 9)
			file << "  ";
		else
			file << "   ";
		file << endl << endl;
		if (forascending->getTYPE() == 0 || forascending->getTYPE() == 1 || forascending->getTYPE() == 2) {
			Total_Df_E += Df;
			Total_Dd_E += Dd;
			Total_Db_E += Db;
			counter_E_for_Ave++;
		}
		else {
			Total_Df_A += Df;
			Total_Dd_A += Dd;
			Total_Db_A += Db;
			counter_A_for_Ave++;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int eartharmyunits = earthArmy.getEScount() + counterForUML1 + killedcountEs + earthArmy.getETcount() + counterForUML2 + killedcountEt +
		earthArmy.getEGcount() + killedcountEg;

	file << "================= Earth Army =================" << endl;
	file << "Battle Result:";
	int earthcount = earthArmy.getEScount() + earthArmy.getEGcount() + earthArmy.getETcount()+counterForUML1+counterForUML2;
	int aliencount = alienArmy.getADcount() + alienArmy.getAMcount() + alienArmy.getAScount();
	if (earthcount == 0 && aliencount == 0)
		file << "  Drawn" << endl;
	else if (earthcount == 0)
		file << "  Loss" << endl;
	else if (aliencount == 0)
		file << "  Win" << endl;
	else if (earthArmy.getEGcount() != 0 && alienArmy.getAScount() != 0 && (earthcount - earthArmy.getEGcount()) == 0 &&
		(aliencount - alienArmy.getAScount()) == 0)
		file << " Drawn" << endl;
	else if (earthArmy.getEScount() != 0 && counterForUML1 != 0 && alienArmy.getADcount() != 0 && (earthcount - earthArmy.getEScount() - counterForUML1) == 0 &&
		(aliencount - alienArmy.getADcount()) == 0)
		file << " Drawn" << endl;

	file << endl;
	file << "Total_Es = " << earthArmy.getEScount() + counterForUML1 + killedcountEs << endl;
	file << "Total_Et = " << earthArmy.getETcount() + counterForUML2 + killedcountEt << endl;
	file << "Total_Eg = " << earthArmy.getEGcount() + killedcountEg << endl << endl;


	double percentage1 = 0, percentage2 = 0, percentage3 = 0;

	int Total_Es = (earthArmy.getEScount() + counterForUML1 + killedcountEs);
	if (Total_Es != 0) {
		percentage1 = ((double)killedcountEs / Total_Es);
		file << "Es_Destructed/Es_Total = " << percentage1 * 100 << "%" << endl;
	}
	else
		file << "Es_Destructed/Es_Total = " << "Not defined" << endl;

	int Total_Et = (earthArmy.getETcount() + counterForUML2 + killedcountEt);
	if (Total_Et != 0) {
		percentage2 = ((double)killedcountEt / Total_Et);
		file << "Et_Destructed/Et_Total = " << percentage2 * 100 << "%" << endl;
	}
	else
		file << "Et_Destructed/Et_Total = " << "Not defined" << endl;

	int Total_Eg = (earthArmy.getEGcount() + killedcountEg);
	if (Total_Eg != 0) {
		percentage3 = ((double)killedcountEg / Total_Eg);
		file << "Eg_Destructed/Eg_Total = " << percentage3 * 100 << "%" << endl;
	}
	else
		file << "Eg_Destructed/Eg_Total = " << "Not defined" << endl;

	int destructed_Earth = killedcountEs + killedcountEt + killedcountEg;
	if (eartharmyunits != 0) {
		file << "Total_Destructed/Total_Units = " << ((double)destructed_Earth / eartharmyunits) * 100 << "%" << endl << endl;
	}
	else
		file << "Total_Destructed/Total_Units = " << "Not defined" << endl << endl;

	if (counter_E_for_Ave > 0) {
		file << "Average_of_DF = " << (double)Total_Df_E / counter_E_for_Ave << endl;
		file << "Average_of_Dd = " << (double)Total_Dd_E / counter_E_for_Ave << endl;
		file << "Average_of_Db = " << (double)Total_Db_E / counter_E_for_Ave << endl << endl;
	}
	else {
		file << "Average_of_DF = " << "Not defined" << endl;
		file << "Average_of_Dd = " << "Not defined" << endl;
		file << "Average_of_Db = " << "Not defined" << endl << endl;
	}
	if (Total_Db_E > 0) {
		file << "Df/Db = " << (double)Total_Df_E / Total_Db_E << "%" << endl;
		file << "Dd/Db = " << (double)Total_Dd_E / Total_Db_E << "%" << endl << endl;
	}
	else {
		file << "Df/Db = " << "Not defined" << endl;
		file << "Dd/Db = " << "Not defined" << endl << endl;
	}
	if (eartharmyunits != 0) {
		file << "Healed_successfully/Total_Earth = " << (double)num_of_healed / eartharmyunits << "%" << endl << endl;
	}
	else
		file << "Healed_successfully/Total_Earth = " << "Not defined" << endl << endl;

	//assume total_Earth= dead + alive
	int earthSunits = earthArmy.getEScount() + counterForUML1+killedcountEs ;
	if (earthSunits != 0)
		file <<"Infected/Total_Es = " << ((double)infectioncount / earthSunits) * 100 << " %" << endl;
	else
		file << "Infected/Total_Es = " << "Not defined" << endl;
file<< endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	int alienarmyunits = alienArmy.getAScount() + killedcountAs + alienArmy.getAMcount() + killedcountAm + alienArmy.getADcount() + killedcountAd;

	file << "============== Alien Army ============" << endl;
	file << "Battle Result:";
	if (earthcount == 0 && aliencount == 0)
		file << "  Drawn" << endl;
	else if (earthcount == 0)
		file << "  Win" << endl;
	else if (aliencount == 0)
		file << "  Loss" << endl;
	else if (earthArmy.getEGcount() != 0 && alienArmy.getAScount() != 0 && (earthcount - earthArmy.getEGcount()) == 0 &&
		(aliencount - alienArmy.getAScount()) == 0)
		file << " Drawn" << endl;
	else if (earthArmy.getEScount() != 0 && alienArmy.getADcount() != 0 && (earthcount - earthArmy.getEScount()) == 0 &&
		(aliencount - alienArmy.getADcount()) == 0)
		file << " Drawn" << endl;
	file << endl;

	file << "Total_AS = " << alienArmy.getAScount() + killedcountAs << endl;
	file << "Total_AM = " << alienArmy.getAMcount() + killedcountAm << endl;
	file << "Total_AD = " << alienArmy.getADcount() + killedcountAd << endl << endl;


	int Total_As = (alienArmy.getAScount() + killedcountAs);
	if (Total_As != 0) {
		percentage1 = ((double)killedcountAs / Total_As);
		file << "As_Destructed/As_Total = " << percentage1 * 100 << "%" << endl;
	}
	else
		file << "As_Destructed/As_Total = " << "Not defined" << endl;

	int Total_Am = (alienArmy.getAMcount() + killedcountAm);
	if (Total_Am != 0) {
		percentage2 = ((double)killedcountAm / Total_Am);
		file << "Am_Destructed/Am_Total = " << percentage2 * 100 << "%" << endl;
	}
	else
		file << "Am_Destructed/Am_Total = " << "Not defined" << endl;

	int Total_Ad = (alienArmy.getADcount() + killedcountAd);
	if (Total_Ad != 0) {
		percentage3 = ((double)killedcountAd / Total_Ad);
		file << "Ad_Destructed/Ad_Total = " << percentage3 * 100 << "%" << endl;
	}
	else
		file << "Ad_Destructed/Ad_Total = " << "Not defined" << endl;

	int destructed_alien = killedcountAd + killedcountAm + killedcountAs;
	
	if (alienarmyunits != 0) {
		file << "Total_Destructed/Total_Units = " << ((double)(destructed_alien) / alienarmyunits) * 100 << "%" << endl << endl;
	}else
		file << "Total_Destructed/Total_Units = " << "Not defined" << endl << endl;

	if (counter_A_for_Ave > 0) {
		file << "Average_of_DF = " << (double)Total_Df_A / counter_A_for_Ave << endl;
		file << "Average_of_Dd = " << (double)Total_Dd_A / counter_A_for_Ave << endl;
		file << "Average_of_Db = " << (double)Total_Db_A / counter_A_for_Ave << endl << endl;
	}
	else {
		file << "Average_of_DF = " << "Not defined" << endl;
		file << "Average_of_Dd = " << "Not defined" << endl;
		file << "Average_of_Db = " << "Not defined" << endl << endl;
	}
	if (Total_Db_A > 0) {
		file << "Df/Db = " << (double)Total_Df_A / Total_Db_A << "%" << endl;
		file << "Dd/Db = " << (double)Total_Dd_A / Total_Db_A << "%" << endl << endl;
	}
	else {
		file << "Df/Db = " << "Not defined" << endl;
		file << "Dd/Db = " << "Not defined" << endl << endl;
	}
	file.close();
}


void Game::infectionspread()
{
	
	Unit* temp=nullptr, *xx;
	LinkedQueue <Unit*> q,collect;
	while (earthArmy.RemoveEarthSoldier(temp))
	{
		srand(time(NULL));
		int prob = rand() % 100+1;


		if (temp->getinfection() && prob <= 2)
		{
			q.enqueue(temp);

			
			while(true){

				temp = nullptr;

				int randoms;
				if (earthArmy.getEScount() != 0)
					randoms = rand() % earthArmy.getEScount() + 1;
				else {
					break;
				}

			

			while (earthArmy.RemoveEarthSoldier(temp)) 
				collect.enqueue(temp);

			while (randoms--) {
				collect.dequeue(temp);
				if(randoms!=0)
				earthArmy.addEarthSoldier(temp);
			}

			while (collect.dequeue(xx))
				earthArmy.addEarthSoldier(xx);



			
			
				if (temp->getinfection())
				
					q.enqueue(temp);
				

				else
				{


					if (!(temp->getimmunity()))
					{
						temp->setinfection(true);
						q.enqueue(temp);
						infectioncount++;
						Real_infection_count++;
						break;
					}
					else
					
						q.enqueue(temp);
				
				}
	
			}
		}
		else
		{
			q.enqueue(temp);
		}

	}
	while (q.dequeue(temp))
	{
		earthArmy.addEarthSoldier(temp);

	}
}

bool Game::needSU()
{
	Unit* unit = nullptr;
	if(Real_infection_count==0)
		while(alliedarmy.RemoveSU(unit))
		{
			delete unit;
		}

	int earthSunits = earthArmy.getEScount() + counterForUML1 ;
	double percentageofinfected;
	if (earthSunits != 0)
		percentageofinfected = (double(Real_infection_count) / earthSunits) * 100.0;
	else
		percentageofinfected = 0;


	
	if (percentageofinfected >= infectionthreshold)
		return true;
	else return false;

}

Game::~Game() {
	Unit* Delete = nullptr;
	while (KilledList.dequeue(Delete)) {
		delete Delete;
	}
	killedcountEs = killedcountEt = killedcountEg = killedcountAs = killedcountAm = killedcountAd = 0;
	int f = 0;
	while (Uml1.dequeue(Delete,f))
		delete Delete;
	while (Uml2.dequeue(Delete))
		delete Delete;
	counterForUML1 = counterForUML2 = 0;
}