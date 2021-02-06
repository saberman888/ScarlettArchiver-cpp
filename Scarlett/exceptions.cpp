#include "exceptions.hpp"
#include <iostream>

namespace ScarlettArchiver
{
	const char* PostRetrievalFailure::what() const
	{
		return "PostRetrievalFailure: exception triggered";
	}

	const char* ScarlettException::what() const
	{
		return message;
	}

	const char* ScarlettPostException::what() const
	{
		return "ScarlettPostException: exception triggered";
	}

	void printException(const ScarlettPostException& e, int level) {
		std::cerr << std::string(level, ' ') << "exception: " << e.what() << " @ Point of error: " << e.postId <<  std::endl;
		try {
			std::rethrow_if_nested(e);
		}
		catch (const ScarlettPostException& ex) {
			printException(ex, level + 1);
		}
		catch (const PostRetrievalFailure& ex) {
			printException(ex, level + 1);
		}
		catch (const std::exception& ex) {
			printException(ex, level + 1);
		}
		catch (...) { throw; }
	}
	void printException(const PostRetrievalFailure& e, int level) {
		std::cerr << std::string(level, ' ') << "exception: " << e.what() << std::endl;
		std::cerr << "HTTP Code: " << e.respinfo.HttpState << std::endl;
		try {
			std::rethrow_if_nested(e);
		}
		catch (const ScarlettPostException& ex) {
			printException(ex, level + 1);
		}
		catch (const PostRetrievalFailure& ex) {
			printException(ex, level + 1);
		}
		catch (const std::exception& ex) {
			printException(ex, level + 1);
		}
		catch (...) { throw; }
	}	
	
	void printException(const ScarlettException& e, int level)
	{
		std::cerr << std::string(level, ' ') << "exception: " << e.what() << std::endl;
		try {
			std::rethrow_if_nested(e);
		}
		catch (const ScarlettPostException& ex) {
			printException(ex, level + 1);
		}
		catch (const PostRetrievalFailure& ex) {
			printException(ex, level + 1);
		}
		catch (const std::exception& ex) {
			printException(ex, level + 1);
		}
		catch (...) { throw; }
	}

	void printException(const std::exception& e, int level)
	{
		std::cerr << std::string(level, ' ') << "exception: " << e.what() << std::endl;
		try {
			std::rethrow_if_nested(e);
		}
		catch (const ScarlettPostException& ex) {
			printException(ex, level + 1);
		}
		catch (const PostRetrievalFailure& ex) {
			printException(ex, level + 1);
		}
		catch (const std::exception& ex) {
			printException(ex, level + 1);
		}
		catch (...) { throw;  }
	}
};