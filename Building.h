#pragma once
#include "BuildingUpdater.h"
#include <iostream>
class Building: public IUpdater
{
	static int _id;
	int _maxAge{};
	int _initialCost{};
	int _age{};
	int _currentId{};
	void destroy();
public:
	Building() = delete;
	Building(int maxAge, int initialCost);
	~Building() = default;
	int getCost() const;
	int getCurrentId() const;
	int getAge() const;
	int getMaxAge() const;
	bool checkAgeStatus() override;
	void toAge();
};