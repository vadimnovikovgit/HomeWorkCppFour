#include "Building.h"
#include <iostream>
Building::Building(int maxAge, int initialCost) : _maxAge{ maxAge }, _initialCost{ initialCost }, _currentId{++_id}
{
}

int Building::getCost() const
{
	if (_age == 0) {
		return _initialCost;
	}
	if (_age >= _maxAge) {
		return 0;
	}

	return _initialCost / _age;
}

int Building::getCurrentId() const
{
	return _currentId;
}

int Building::getAge() const
{
	return _age;
}

void Building::toAge()
{
	_age++;
	
}

bool Building::checkAgeStatus()
{
	if (_age >= _maxAge) {
		destroy();
		return false;
	}
	return true;
}


void Building::destroy()
{
	std::cout << "Building " << _currentId << " was destroyed\n";
}

int Building::_id{ 0 };