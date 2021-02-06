#pragma once

#include <string>
#include <sstream>
#include <iomanip>

#include "BasicRequest.hpp"
#include "Logger.hpp"

/*
* A header file for misc functions and single functions that don't fit in any other header file
*/

namespace ScarlettArchiver
{	
	/**
	* just like the normal strptime on Linux. I just needed an independent implementation because, MSVC doesn't have one
	* @author Sai Vazquez
	* @params Date in string format, the date's format and a struct tm*
	*/
	char* strptime(const char* s, const char* f, struct tm* tm);

	State Download(const std::string URL);



}
