#include"Unit.h"
#include<iostream>
using namespace std;

Unit::Unit(double h, double pw, double AC, int tj) :Health(h), Power(pw), AttackCap(AC), Tj(tj)
{
	ID = -1;
	setTa(-1);
	setTd(-1);
	initialhealth = h;
	timeUml = -1;
	infected = false;
	immunity = false;
}

void Unit::Attack(Unit*&a)
{
	if (a->getHealth() > 0) {
		int damage = (getPower() * getHealth() / 100) / sqrt(a->getHealth());
		a->setHealth(a->getHealth() - damage);
	}
}


void Unit::setID(int id)
{
	ID = id;
}

void Unit::settype(int t)
{
	type = t;
}

void Unit::setPower(int p)
{
	Power = p;
}

void Unit::setHealth(int h)
{
	Health = h;
}

int Unit::getID() const
{
	return ID;
}

void Unit::setTJ(int t)
{
	Tj = t;
}

int Unit::getPower() const
{
	return Power;
}

int Unit::getHealth() const
{
	return Health;
}

int Unit::getTYPE() const
{
	return type;
}

void Unit::setAttackCap(int x)
{
	AttackCap = x;
}

int Unit::getAttackCap() const
{
	return AttackCap;
}

void Unit::setTa(int t) {
	Ta = t;
}
void Unit::setTd(int t) {
	Td = t;
}
int Unit::getTa()const {
	return Ta;
}
int Unit::getTd()const {
	return Td;
}
int Unit::getTj()const {
	return Tj;
}

int Unit::getinitialhealth()const {
	return initialhealth;
}
void Unit::setinitialhealth(int h) {
	initialhealth = h;
}
int Unit::gettimeUml()const {
	return timeUml;
}
void Unit::settimeUml(int p) {
	timeUml = p;
}

ostream& operator<<(ostream& out, Unit* u)
{
	if (u->getinfection())
	{
		out << u->getID() << "f";
	}
	else
	{
		out << u->getID();
	}
	return out;
	// TODO: insert return statement here
}
void Unit::setinfection(bool a)
{
	infected = a;
}
bool  Unit::getinfection()
{
	return infected;
}
void Unit::setimmunity(bool a)
{
	immunity = a;
}

bool Unit::getimmunity()
{
	return immunity;
}
