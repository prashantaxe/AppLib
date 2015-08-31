#ifndef APP_OBJECTLISTHOLDER_HPP_
#define APP_OBJECTLISTHOLDER_HPP_
#include <stdint.h>

namespace App
{
template<typename Obj>
class AppObjectListHolder
{
enum
{
	MAX_BUFFER_SIZE=1024
};
public:
AppObjectListHolder()
:m_objArray(nullptr)
,m_ArraySize(MAX_BUFFER_SIZE)
,m_readInd(0)
,m_currSize(0)
{
	m_objArray = new Obj[m_ArraySize];
}
~AppObjectListHolder()
{
	delete [] m_objArray;
}
bool push(Obj& val)
{
	if(m_currSize==m_ArraySize)
		return false;

	m_objArray[m_currSize++]=val;
	return true;
}
bool pop(Obj& val)
{
	if(m_readInd>=m_currSize)
		return false;
	val = m_objArray[m_readInd];
	return true;	
}

uint32_t capacity()
{
	return m_ArraySize;
}
uint32_t size()
{
	return m_currSize;
}
void resetRaw(int ind)
{
	m_objArray[ind] = nullptr;
}
void reset(uint32_t ind)
{
	m_objArray[ind].reset();
}
void reset()
{
	m_currSize = m_readInd = 0;
}
Obj& operator[](uint32_t ind)
{
	return m_objArray[ind];
}
protected:
	Obj* m_objArray;
	uint32_t m_ArraySize;
	uint32_t m_currSize;
	uint32_t m_readInd;	
};
};// napespace App
#endif
