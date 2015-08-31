#include "AppMessageQueueMPSC.hpp"
#include <iostream>
#include <thread>

int main()
{
	AppMessageQueueMPSC<int, 8> queue;
	auto call_back = [&queue](){
							int obj;
							while(queue.pop_wait(obj))
								std::cout<<"Popped "<<obj<<"\n";
							return 0;};
	std::thread t1(call_back);
	for(int i=0; i<9; ++i)
		if(queue.push(i)) std::cout<<"Pushed "<<i<<"\n";
	t1.join();
}
