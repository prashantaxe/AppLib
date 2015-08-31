#include "AppSpinLock.hpp"
#include "AppTrace.hpp"
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
int main()
{
	AppSpinLock spinLock;
	std::mutex m;
	auto lamda = [](std::mutex& m){
		while(true)
		{
			{
				//std::lock_guard<AppSpinLock> lock(spinLock);
				std::lock_guard<std::mutex> lock(m);
				TRACE(LOGLVL_ERROR,"Spawned Thread has taken the Lock");
				std::this_thread::sleep_for(std::chrono::seconds(1));
				TRACE(LOGLVL_ERROR, "Spawn Thread Relinquished Lock");
			}
		}
	};

	std::thread t1(lamda, std::ref(m));
//}
//	spinLock.lock();
//	spinLock.unlock();
	while(true)
	{
	//{	std::lock_guard<AppSpinLock> lock(spinLock);
	{	std::lock_guard<std::mutex> lock(m);
		TRACE(LOGLVL_ERROR, "Main Thread Taking the Lock");
		std::this_thread::sleep_for(std::chrono::seconds(1));}
		TRACE(LOGLVL_ERROR, "Main thread relinquished lock");
	}
};
