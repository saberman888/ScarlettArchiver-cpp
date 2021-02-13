#pragma once

#include <string>
#include <exception>
#include <cstring>
#include <iosfwd>
#include "BasicRequest.hpp"

namespace ScarlettArchiver
{
	class ScarlettException : public std::exception
	{
	public:
		ScarlettException(const char* message) : message(message) {}
		ScarlettException(std::string message) : message(message.c_str()){}
		std::string postId;
		const char* message;
		virtual const char* what();
	};

	class ScarlettPostException : public ScarlettException
	{
	public:
		std::string postId;
		ScarlettPostException(const std::string message, const std::string postid) : ScarlettException(message.c_str()), postId(postid){}
		const char* what();
	};

	class PostRetrievalFailure : public ScarlettException	{
	public:
		PostRetrievalFailure(const std::string message) : ScarlettException(message) {}
		PostRetrievalFailure(const State& result) : ScarlettException(result.Message) {
			respinfo.Message = result.Message;
			respinfo.HttpState = result.HttpState;
		}

		State respinfo;
		const char* what();
	};

}
