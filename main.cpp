#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <memory>
#include "Building.h"
#include "BuildingUpdater.h"


using namespace std::chrono_literals;
std::mutex mt;
std::condition_variable cv;
bool agingComplete = false;

int getRandomNumber(int min, int max) {
	std::random_device rd;
	std::mt19937 rng{ rd() };
	std::uniform_int_distribution<int> uid(min, max);
	return uid(rng);
}

void buildHomes(OSBB*& company, int quantity) {
	std::vector<std::shared_ptr<IUpdater>> buildings;
	for (size_t i = 0; i < quantity; i++)
	{
		company->Attach(std::make_shared<Building>(getRandomNumber(2, 12), getRandomNumber(100000, 10000000)));
	}
}

void startAging(std::vector<std::shared_ptr<IUpdater>>& buildings) {
	int years{1};
	while (!buildings.empty()) {
		{
			std::lock_guard<std::mutex> lg{ mt };
			std::cout << "Year " << years << "\n========================================================================\n";
			for (const auto& b : buildings) {
				auto castBuilding{ dynamic_cast<Building*>(b.get()) };
				if (castBuilding == nullptr) {
					continue;
				}
				castBuilding->toAge();				
				std::cout << "Building: " << castBuilding->getCurrentId() << " cost: " << castBuilding->getCost() << " age: " << castBuilding->getAge() << " max age: " << castBuilding->getMaxAge() << std::endl;	
			}
		}
		std::cout << "========================================================================\n";
		years++;
		agingComplete = true;
		cv.notify_all();
		std::this_thread::sleep_for(2000ms);
	}
}

void osbbWorkFlow(OSBB* company, std::vector<std::shared_ptr<IUpdater>>& buildVec) {
	std::unique_lock<std::mutex> lock{ mt };
	while (!company->getBuildingList().empty()) {
		cv.wait(lock, [] { return agingComplete; });
		company->Notify();
		agingComplete = false;
	}
}


int main() {
	OSBB* company{ new OSBB{} };
	buildHomes(company, 10);
	std::thread age{ startAging, std::ref(company->getBuildingList()) };	
	std::thread osbb{ osbbWorkFlow,  std::ref(company), std::ref(company->getBuildingList())};
	age.join();
	osbb.join();

	return 0;
}