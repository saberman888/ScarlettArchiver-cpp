#include "exceptions.hpp"
#include <iostream>

namespace Scarlett
{
	ScarlettException::ScarlettException(const std::string& message, unsigned int line, const std::string& func)
	{
		this->message = "[" + std::string(func) + ":" + std::to_string(line) + "]: " + message;
		std::runtime_error::runtime_error(message);
	}


	ScarlettException::ScarlettException(const std::string&& message, unsigned int line, const std::string& func)
	{
		ScarlettException::ScarlettException(std::forward<const std::string&>(message), line, func);
	}
	void printException(const std::exception& e, int level)
	{
		std::cerr << std::string(level, ' ') << "[exception]: " << e.what() << '\n';		
		auto log = GetGlobalLogger();
		log->error(std::string(level, ' ') + "[exception]: " + e.what());
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

};
