#include "Scarlett/Internal/Exceptions.hpp"
#include <iostream>

namespace Scarlett
{
	ScarlettException::ScarlettException(const std::string& message, unsigned int line, const std::string& func) 
		: 
		std::runtime_error(
			"[" + std::string(func) + ":" + std::to_string(line) + "]: " + message
		)
	{
		this->message = "[" + std::string(func) + ":" + std::to_string(line) + "]: " + message;
	}


	ScarlettException::ScarlettException(const std::string&& message, unsigned int line, const std::string& func) 
		: std::runtime_error(
			"[" + std::string(func) + ":" + std::to_string(line) + "]: " + message
		){
        this->message = "[" + std::string(func) + ":" + std::to_string(line) + "]: " + message;
    }
	
	void printException(const std::exception& e, int level)
	{
		std::cerr << std::string(level, '\t') << "[exception]: " << e.what() << '\n';		
		auto log = GetGlobalLogger();
		log->error(std::string(level, '\t') + "[exception]: " + e.what());
		try {
			std::rethrow_if_nested(e);
			
		}
		catch (const ScarlettException& e) {
			printException(e, level + 1);
		}
		catch (const std::exception& e) {
			printException(e, level + 1);
		}
		catch (...) {

		}
	}

	void printException(ScarlettException& se, int level)
	{
		std::cerr << std::string(level, ' ') << "[scarlett exception]: " << se.what() << std::endl; 
		auto log = GetGlobalLogger();
		log->error(std::string(level, ' ') + "[exception]: " + se.what());
		try {
			std::rethrow_if_nested(se);
		}
		catch (ScarlettException& e) {
			printException(e, level + 1);
		}
		catch (const std::exception& e) {
			printException(e, level + 1);
		}
		catch (...) {
		}
	}

	ScarlettHTTPException::ScarlettHTTPException(const HttpResponse response, unsigned int line, const std::string& func) 
		: 
		ScarlettException(
			response.status_code() + ": " + conv::to_utf8string(response.reason_phrase()),
			line,
			func
		)
	{
		this->response = response;
	}

};
