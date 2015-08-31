#ifndef APP_MESSAGEQUEUE_MPSC_HPP
#define APP_MESSAGEQUEUE_MPSC_HPP
#include "AppSpinLock.hpp"

#include <atomic>
#include <stdint.h>
#include <mutex>
#include <new>

using namespace App;

namespace App
{
enum
{
	CACHELINE_SIZE=64,
	SIZE = 65536
};

template <uint32_t N>
struct is_pow_of_2
{
	enum{val = N && !(N&(N-1))};
	static_assert(val, "N should be a power of 2.");
};
template<class T, uint32_t Q_SIZE=SIZE>
class AppMessageQueueMPSC
{
protected:
	// The intent to use padding is to avoid excessive flushing of cacheline. 
	T* m_buffer;
	char padding1[CACHELINE_SIZE];
	uint32_t m_size;
	char padding2[CACHELINE_SIZE];
	std::atomic<uint32_t> m_readID;
	char padding3[CACHELINE_SIZE];
	std::atomic<uint32_t> m_writeID;
	char padding4[CACHELINE_SIZE];
	AppSpinLock m_lock;

	is_pow_of_2<Q_SIZE> dummy;
	static const uint32_t Q_MASK=Q_SIZE-1;
public:
	AppMessageQueueMPSC(size_t size=SIZE)
	:m_buffer(NULL)
	,m_size(Q_SIZE)
	{
		m_buffer=new T[m_size];
		if(!m_buffer)
			throw std::bad_alloc();
		m_readID.store(0, std::memory_order_release);
		m_writeID.store(0, std::memory_order_release);
	}
	bool push(T& obj)
	{
		std::lock_guard<AppSpinLock> lock(m_lock);
		uint32_t writeId = m_writeID.load(std::memory_order_acquire);
		uint32_t readId  = m_readID.load(std::memory_order_acquire);
		if(writeId<readId+m_size)
		{
			m_buffer[writeId&Q_MASK] = obj;
			m_writeID.fetch_add(1, std::memory_order_acq_rel);
			return true;
		}
		else
		{
			return false;
		}
	}
	bool pop(T& obj)
	{
		std::lock_guard<AppSpinLock> lock(m_lock);
		uint32_t writeId = m_writeID.load(std::memory_order_acquire);
		uint32_t readId  = m_readID.load(std::memory_order_acquire);
		if(readId<writeId)
		{
			obj = m_buffer[readId&Q_MASK];
			m_readID.fetch_add(1, std::memory_order_acq_rel);
			return true;
		}
		return false;
	}
	bool pop_wait(T& obj)
	{
		std::lock_guard<AppSpinLock> lock(m_lock);
		uint32_t writeId = m_writeID.load(std::memory_order_acquire);
		uint32_t readId  = m_readID.load(std::memory_order_acquire);
		//try{	I need to write an interrupt mechanism...
			while(readId>=writeId)
			{
				//Pause Statement
				__asm volatile("pause":::"memory");
				writeId = m_writeID.load(std::memory_order_acquire);
				readId  = m_readID.load(std::memory_order_acquire);
			}
		//}catch(std::interrupt)
		obj = m_buffer[readId&Q_MASK];
		m_readID.fetch_add(1, std::memory_order_acq_rel);
		return true;
	}
};
};//End of namespace App
#endif
