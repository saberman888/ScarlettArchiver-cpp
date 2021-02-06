#include "PushShift.hpp"
#include <fstream>
#include <iostream>
namespace PushShift {

	static State PushShiftGet(std::string endpoint, const std::map<std::string, std::string>& URLComponents)
	{
        BasicRequest handle;
        State result;
        handle.Setup("https://api.pushshift.io" + endpoint);
        handle.SetURLParameters(URLComponents);
        result = handle.SendRequest();
        handle.Cleanup();
		return result;
	}
	
	nlohmann::json SearchSubmissions(const std::map<std::string, std::string>& URLComponents)
	{
		State result = PushShiftGet("/reddit/submission/search", URLComponents);
		if(result.AllGood())
		{
			return nlohmann::json::parse(result.buffer);
		} else {
			std::cerr << result.HttpState << ": " << result.Message << std::endl;
			throw std::runtime_error("Error failed to retrieve data");
		}
		return nlohmann::json();
	}
	nlohmann::json SearchComments(const std::map<std::string, std::string>& URLComponents)
	{
		State result = PushShiftGet("/reddit/search/comment", URLComponents);
		if (result.AllGood())
		{
			return nlohmann::json::parse(result.buffer);
		}
		else {
			std::cerr << result.HttpState << ": " << result.Message << std::endl;
			throw std::runtime_error("Error failed to retrieve data");
		}
		return nlohmann::json();
	}
};
