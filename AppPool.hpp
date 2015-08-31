#ifndef APP_POOL_HPP
#define APP_POOL_HPP
#include "AppSpinLock.hpp"

#include <mutex>
#include <list>
#include <memory>
#include <functional>

enum
{
	GROW_SIZE=2048,
	INIT_SIZE=65536,
};
namespace App
{
template<class T>
class AppPool
{
	std::list<T*> m_freeList;
	std::list<T*> m_allocList;
	
	int64_t m_initSize;
	int64_t m_growSize;
	AppSpinLock m_lock;

public:

	AppPool()
	:m_initSize(INIT_SIZE)
	,m_growSize(GROW_SIZE)
	{}
	~AppPool()
	{
		typename std::list<T*>::iterator iter = m_freeList.begin();
		typename std::list<T*>::iterator iter_END = m_freeList.end();
		for( ; iter!=iter_END; ++iter)
		{
			T* temp = *iter;
			delete[] temp;
		}
	}
	bool init(int64_t initSize=INIT_SIZE,
			  int64_t growSize=GROW_SIZE)
		{
			T* tempPtr = reinterpret_cast<T*>(new char[INIT_SIZE*sizeof(T)]);
			if(!tempPtr)
			{
				
				throw std::runtime_error("Memory Allocation failed for Object Pool.");				
			}
			for(int64_t i=0; i<INIT_SIZE; ++i)
			{
				m_allocList.push_back(tempPtr+i);
			}
			m_freeList.push_back(tempPtr);
			return true;
		}
		T* allocPtrObj()
		{
			std::lock_guard<AppSpinLock> lock(m_lock);
			T* ptr = NULL;
			if(!m_allocList.empty())
			{
				ptr = m_allocList.front();
				m_allocList.pop_front();
			}
			else
			{
				//Expand the Pool
			}
			return ptr;
		}
		std::shared_ptr<T>
		allocSharedPtr()
		{
			std::lock_guard<AppSpinLock> lock(m_lock);
			std::shared_ptr<T> ptr;
			if(!m_allocList.empty())
			{
				T* temp = m_allocList.front();
				std::function<void (T*)> deleter = 
				std::bind(&AppPool::release, this, std::placeholders::_1);
				ptr.reset(temp, deleter);
			}
			return ptr;
		}
		void release(T* ptr)
		{
			std::lock_guard<AppSpinLock> lock(m_lock);
			m_allocList.push_back(ptr);
		}	
};
};
#endif
