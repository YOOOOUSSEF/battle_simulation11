#include "AlliedArmy.h"

AlliedArmy::AlliedArmy()
{
	SUcount = 0;
	id = 4000;
}
void AlliedArmy:: addSU(Unit* soldier)
{
	SU.enqueue(soldier);
	if (soldier->getID() == -1)
	{
		soldier->setID(id);
		id++;
	}
	SUcount++;
}
bool AlliedArmy::RemoveSU(Unit*& Soldier)
{
	if (SUcount == 0)
	{
		Soldier= NULL;
		return false;
	}
	SU.dequeue(Soldier);//dequeuing the Soldiers 
	SUcount--;
	return true;
}
void AlliedArmy::printSU()
{
	cout << SUcount << " SU [";
	SU.print_the_list();
	cout << "] ";
	cout << endl;
}
int AlliedArmy::getSUcount()
{
	return SUcount;
}
AlliedArmy::~AlliedArmy()
{
	Unit* Delete;
	while (!SU.isEmpty())
	{
		SU.dequeue(Delete);
		delete Delete;
	}


	id = 4000;
	SUcount = 0;

}
