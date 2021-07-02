#include "Scarlett/Reddit/SubredditMetadata.hpp"
#include <iostream>
//BOOST_CLASS_EXPORT_KEY2(Scarlett::Reddit::SubredditMetadata, "metadata")
BOOST_CLASS_EXPORT_IMPLEMENT(Scarlett::Reddit::SubredditMetadata)

namespace Scarlett::Reddit
{ 
	
	SubredditMetadata::SubredditMetadata(const String Subreddit, const String StartTime, const String EndTime)
	{
		InitializeDates(StartTime, EndTime);
		this->_Subreddit = Subreddit;
	}

	void SubredditMetadata::InitializeDates(std::optional<String> Start, std::optional<String> End)
	{		

		const auto match = [&Start, &End](const std::string pattern) -> bool { return (std::regex_match(toString(*Start), std::regex(pattern)) && std::regex_match(toString(*End), std::regex(pattern))); };

		// Check if the Start date and End date are present
		if (!Start)
		{
			// If Start is not present, assign it to when Reddit first emerged since we have no idea when the Subreddit emerged
			tm autoDate{ 0 };
			autoDate.tm_year = 2005;
			autoDate.tm_mday = 23;
			autoDate.tm_mon = 07;

			StartDate = mktime(&autoDate);
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

			if (Start) {
				tm tempStart{ 0 };
				Scarlett::strptime(toString(*Start).c_str(), format.c_str(), &tempStart);
				StartDate = mktime(&tempStart);
			}
			if (End) {
				tm tempEnd{ 0 };
				Scarlett::strptime(toString(*End).c_str(), format.c_str(), &tempEnd);
				EndDate = mktime(&tempEnd);
			}
		}
		this->DatePointer = this->StartDate;
	}
}