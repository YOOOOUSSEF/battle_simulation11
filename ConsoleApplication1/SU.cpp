#include "SU.h"
SU::SU(double h, double pw, double AC, int tj) : Unit(h, pw, AC, tj)
{
	settype(7);
}
void SU::Attack(Unit*& as)
{
	int damage = (getPower() * getHealth() / 100) / sqrt(as->getHealth());
	as->setHealth(as->getHealth() - damage);
}
