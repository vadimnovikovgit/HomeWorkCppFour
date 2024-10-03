#pragma once
#include "Building.h"
#include <list>
#include <vector>
#include <memory>
class IUpdater {
public:
	virtual bool checkAgeStatus() = 0;
};

class Iosbb {
public:
	virtual ~Iosbb() {};
	virtual void Attach(std::shared_ptr<IUpdater> building) = 0;
	virtual std::vector< std::shared_ptr<IUpdater>>::iterator Detach(std::shared_ptr<IUpdater> building) = 0;
	virtual void Notify() = 0;
};

class OSBB: public Iosbb {
	std::vector<std::shared_ptr<IUpdater>> buildingList;
public:
	
	void Attach(std::shared_ptr<IUpdater> building) override {
		buildingList.push_back(building);
	}

	std::vector< std::shared_ptr<IUpdater>>::iterator Detach(std::shared_ptr<IUpdater> building) override {
		return buildingList.erase(std::find(buildingList.begin(), buildingList.end(), building));
	}

	void Notify() override {
		/*for (auto& b: buildingList) {
			if (!b->checkAgeStatus()) {
				Detach(b);
			}
		}*/
		for (auto it{ buildingList.begin() }; it != buildingList.end();) {
			if (!(*it)->checkAgeStatus()) {
				it = Detach(*it);
				continue;
			}
			it++;
		}

	}
	void registerBuildings(std::vector<std::shared_ptr<IUpdater>>& buildings) {
		for (auto& building : buildings) {
			Attach(building);
		}
	}
	std::vector<std::shared_ptr<IUpdater>>& getBuildingList() {
		return buildingList;
	}
};