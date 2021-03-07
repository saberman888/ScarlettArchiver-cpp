#include "SubredditMetadata.hpp"
#include <iostream>

namespace Scarlett::Reddit
{ 
	
	SubredditMetadata::SubredditMetadata(const struct ScarlettOptions::POptions ops)
	{
		InitializeDates(ops.StartTime, ops.EndTime);
		Subreddit = ops.Subreddit;
	}

	void SubredditMetadata::InitializeDates(std::optional<std::string> Start, std::optional<std::string> End)
	{		

		const auto match = [&Start, &End](const std::string pattern) -> bool { return (std::regex_match(*Start, std::regex(pattern)) && std::regex_match(*End, std::regex(pattern))); };

		// Check if the Start date and End date are present
		if (!Start)
		{
			// If Start is not present, assign it to when Reddit first emerged since we have no idea when the Subreddit emerged
			this->StartDate.tm_year = 2005;
			this->StartDate.tm_mday = 23;
			this->StartDate.tm_mon = 07;
			if (!End)
			{
				// If End isn't either, assign it to today's date in UTC
				const time_t current = 0;
				this->EndDate = *std::gmtime(&current);
			}
		}
		else {
			/*
			* Supported formats:
			* 
			* 1. YYYY:MM:DD
			* 2. YYYY.MM.DD
			* 3. YYYY-MM-DD
			* 4. YYYMMDD
			* 5. 2020/01/01
			
			*/
			std::string format;
			if (match("20[0-9][0-9](-|\\.|:|\\/)?[0-1]?[1-9](-|\\.|:|\\/)?[0-3]?[0-9]"))
			{
				if (match("20[0-9][0-9][0-1][0-9][0-3][0-9]"))
				{
					format = "%Y%m%d";
				}
				else if (match("20[0-9][0-9]-[0-1][1-9]-[0-3][0-9]")) {
					format = "%Y-%m-%d";
				}
				else if (match("20[0-9][0-9]:[0-1][1-9]:[0-3][0-9]")) {
					format = "%Y:%m:%d";
				}
				else if (match("20[0-9][0-9].[0-1][1-9].[0-3][0-9]")) {
					format = "%Y.%m.%d";
				}
				else if (match("20[0-9][0-9]/[0-1][1-9]/[0-3][0-9]")) {
					format = "%Y/%m/%d";
				}
				else {
					throw std::runtime_error("Not a valid date format");
				}
			}
			else {
				throw std::runtime_error("Not a valid date format");
			}
			Scarlett::strptime(Start->c_str(), format.c_str(), &this->StartDate);
			Scarlett::strptime(End->c_str(), format.c_str(), &this->EndDate);
		}
		this->DatePointer = this->StartDate;
	}	
}