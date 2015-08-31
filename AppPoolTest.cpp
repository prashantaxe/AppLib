#include <iostream>
#include "AppPool.hpp"

int main()
{
	App::AppPool<int> pool;
	pool.init();
	std::shared_ptr<int> ptr =  pool.allocSharedPtr();
}
