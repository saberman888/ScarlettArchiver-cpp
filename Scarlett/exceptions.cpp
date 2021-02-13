#include "exceptions.hpp"
#include <iostream>

namespace ScarlettArchiver
{
	const char* PostRetrievalFailure::what()
	{
		return "PostRetrievalFailure: exception triggered";
	}

	const char* ScarlettException::what()
	{
		return message;
	}

	const char* ScarlettPostException::what()
	{
		return "ScarlettPostException: exception triggered";
	}

};