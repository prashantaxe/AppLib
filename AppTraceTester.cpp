#include "AppTrace.hpp"

int main()
{
//	AppTrace traceObj;
	int a = 123;
	//traceObj.print( "hello", a);
	TRACE(LOGLVL_ERROR, "Hello", 123);
	return 0;
}

