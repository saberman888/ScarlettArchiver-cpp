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
    class SCDLL ScarlettException : public std::runtime_error
	{
	public:
		ScarlettException(const std::string& message, unsigned int line, const std::string& func);
		ScarlettException(const std::string&& message, unsigned int line, const std::string&& func);

		const char* what() const noexcept
		{
			return message.c_str();
		}

		inline const int line()
		{
			return loc;
		}

		inline const std::string func()
		{
			return _func;
		}

	private:
		int loc;
		std::string _func, message;
	};


    class SCDLL ScarlettHTTPException : public ScarlettException
	{
	public:
		ScarlettHTTPException(const HttpResponse& response, unsigned int line, const std::string& func);

		inline const HttpResponse Response()
		{
			return response;
		}
	private:
		HttpResponse response;
	};

	class ScarlettOutOfTimeException : public ScarlettException
	{
		public:
			ScarlettOutOfTimeException(const std::string&& message, int line, std::string&& func) : ScarlettException(message, line, func){}
	};


    void SCDLL printException(const std::exception& e, int level = 0);
    void SCDLL printException(ScarlettException& se, int level = 0);

#define scarlettThrow(msg) throw ScarlettException(msg, __LINE__, __func__);
#define scarlettNestedThrow(msg) std::throw_with_nested(ScarlettException(msg, __LINE__, __func__));
#define scarlettHTTPThrow(response) throw ScarlettHTTPException(response, __LINE__, __func__);
#define scarlettHTTPNestedThrow(response) std::throw_with_nested(ScarlettHTTPException(response, __LINE__, __func__));
#define scarlettOOTThrow(msg) throw ScarlettOutOfTimeException(msg, __LINE__, __func__);
}
