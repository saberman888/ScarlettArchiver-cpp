#pragma once

#include <stdexcept>
#include <exception>
#include <iosfwd>
#include "Logger.hpp"
#include <utility>
#include "Helpers.hpp"

#if defined(__FUNCTION__)
#define __func__ __FUNCTION__
#endif

namespace Scarlett
{
	class ScarlettException : public std::runtime_error
	{
	public:
		ScarlettException(const std::string& message, unsigned int line, const std::string& func);
		ScarlettException(const std::string&& message, unsigned int line, const std::string& func);

		const char* what()
		{
			return message.c_str();
		}
	private:
		std::string message;
	};


	class ScarlettHTTPException : public ScarlettException
	{
	public:
		ScarlettHTTPException(const HttpResponse response, unsigned int line, const std::string& func);

		inline const HttpResponse& Response()
		{
			return response;
		}
	private:
		HttpResponse response;
	};


	void printException(const std::exception& e, int level = 0);
	void printException(ScarlettException& se, int level = 0);

#define scarlettThrow(msg) throw ScarlettException(msg, __LINE__, __func__);
#define scarlettNestedThrow(msg) std::throw_with_nested(ScarlettException(msg, __LINE__, __func__));

}
