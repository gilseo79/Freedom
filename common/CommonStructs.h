#pragma once

#include <stdexcept>
#include <string>

struct timeout_error: public std::exception
{
	explicit timeout_error(unsigned int timeoutMiliSec);
	const char * what () const throw ();
	private:
		std::string		reason_;
};


