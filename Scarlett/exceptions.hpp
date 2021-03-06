#pragma once

#include <stdexcept>
#include <exception>
#include <iosfwd>
#include "Logger.hpp"

#if defined(__FUNCTION__)
#define __func__ __FUNCTION__
#endif

namespace ScarlettArchiver
{
	class ScarlettException : public std::runtime_error
	{
	public:
		ScarlettException(const std::string& message, unsigned int line, const std::string& func) : std::runtime_error("[" + std::string(func) + ":" + std::to_string(line) + "]: " + message){
			this->message = "[" + std::string(func) + ":" + std::to_string(line) + "]: " + message;
		}
		const char* what()
		{
			return message.c_str();
		}
	private:
		std::string message;
	};


	void printException(const std::exception& e, int level = 0);
	void printException(ScarlettException& se, int level = 0);

#define scarlettThrow(msg) throw ScarlettException(msg, __LINE__, __func__);
#define scarlettNestedThrow(msg) std::throw_with_nested(ScarlettException(msg, __LINE__, __func__));

}
