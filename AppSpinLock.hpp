#ifndef APP_SPINLOCK_HPP
#define APP_SPINLOCK_HPP
#include <atomic>
#include <stdint.h>
#include <iostream>

namespace App{
class spin_lock
{
	enum
	{
		UNLOCKED=0,
		LOCKED=1
	};
	std::atomic<int32_t> m_lock;
	public:
	spin_lock()
	{
		if(m_lock.is_lock_free())
			std::cout<<"Implementation is Lockfree\n";

		else std::cout<<"Implementation is not Lockfree\n";
		
		m_lock.store(UNLOCKED, std::memory_order_release);
	};
	void lock()
	{
		int state = UNLOCKED;
		do
		{
			if(m_lock.compare_exchange_strong(state, LOCKED, std::memory_order_acq_rel))
				break;
		}
		while(m_lock.load(std::memory_order_acquire)!=UNLOCKED)
	}
	void unlock()
	{
		m_lock.store(UNLOCKED, std::memory_order_release);
	}
};

typedef spin_lock AppSpinLock;
};
#endif
