#ifndef APP_TRACE_HPP
#define APP_TRACE_HPP
#include <sstream>
#include <iostream>

enum LOGLVL
{
	LOGLVL_ERROR,
	LOGLVL_NOTIFY,
	LOGLVL_DEBUG
};

class AppTrace
{
	//This function is execued in a separate thread called Logger Thread.
	void run()
	{
	}
	template<typename T>
	void printUtil(std::stringstream& ss, const T& t)
	{
		ss<<t;
		return;
	}
	template<typename T, typename... Arg>
	void printUtil(std::stringstream& ss, const T& t, const Arg&... args)
	{
		ss<<t;
		printUtil(ss, args...);
		return;
	}
	AppTrace()
	:log_lvl(0)
	{}
	public:
	template<typename... Arg>
	void print(const char* file, int line, 
						const Arg&... args)
	{
		std::stringstream ss;
		printUtil(ss, args...);
		std::cout<<"["<<line<<"]"<<"["<<file<<"]"<<ss.str()<<"\n";
		std::cout.flush();
	}
	static AppTrace getAppTrace()
	{
		static AppTrace inst;
		return inst;
	}
	int getDebugLevel()
	{
		return log_lvl;
	}
	int log_lvl;
};

#define TRACE(debug_lvl, ...)										\
if(debug_lvl<=AppTrace::getAppTrace().getDebugLevel())				\
	AppTrace::getAppTrace().print(__FILE__, __LINE__, __VA_ARGS__)

#endif
