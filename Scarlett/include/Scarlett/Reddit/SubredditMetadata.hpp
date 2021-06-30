#pragma once

#include <iosfwd>
#include <optional>
#include <regex>
#include "Video.hpp"
#include "SelfPost.hpp"
#include "Galleries.hpp"


namespace Scarlett::Reddit
{

	/*
		SubredditMetadata holds information regarding the date, position, statistics and functions manipulating around a Subreddit
	*/
    class SCDLL SubredditMetadata : protected Logger
	{
	public:

		/*
			Pass Boost program options to the constructor
		*/
		SubredditMetadata(const String Subreddit, const String StartTime, const String EndTime);
		SubredditMetadata() = default;


		inline void setSubreddit(const String& name)
		{
			_Subreddit = name;
		}

		inline const String Subreddit()
		{
			return _Subreddit;
		}

		inline void setStart(const time_t date)
		{
			StartDate = date;
		}

		inline const time_t Start()
		{
			return StartDate;
		}

		inline void setEnd(const time_t date)
		{
			EndDate = date;
		}

		inline const time_t End()
		{
			return EndDate;
		}

		inline const time_t Position()
		{
			return DatePointer;
		}

		inline void setPosition(const time_t date)
		{
			DatePointer = date;
		}

	private:
		String _Subreddit;
		time_t StartDate{ 0 }, EndDate{ 0 }, DatePointer{ 0 };
		friend boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("TargetSubreddit", _Subreddit);
			ar& make_nvp("StartDate", StartDate);
			ar& make_nvp("EndDate", EndDate);
			ar& make_nvp("DatePointer", DatePointer);
		}

		/**
		 Scans the provided dates in string format into a Time Structure , and assigns them to
		 StartDate and EndDate. It uses the strptime provided in Misc.hpp
		
		 @param Dates in string format with the provided date formats: YYYY:MM:DD, YYYY-MM-DD, YYYY.MM.DD, YYYY/MM/DD or YYYYMMDD
		*/
		void InitializeDates(std::optional<String> Start = std::nullopt, std::optional<String> End = std::nullopt);
	};
};
