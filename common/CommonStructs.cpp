#include "CommonStructs.h"

using namespace std;

timeout_error::timeout_error(unsigned int timeoutMiliSec)
{
	reason_ = "timeout : " + to_string(timeoutMiliSec) + " msec";
}

const char * timeout_error::what () const throw ()
{
	return reason_.c_str();	
}


