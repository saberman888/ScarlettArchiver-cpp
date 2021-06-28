#pragma once

#include <chrono>
#include <optional>
#include <functional>
#include <deque>
#include <tuple>
#include <cpprest/http_client.h>
#include <thread>
#include <algorithm>
#include <exception>
#include <mutex>
#include "../Internal/Helpers.hpp"
#include "../Internal/Exceptions.hpp"
namespace Scarlett::Client
{
	/*
	* RateTracker's duty is to keep track of requests done by classes like PushShift, and make sure they dont break rate limits.
	* 
	* TODO: Re-implement things like total requests, total time and more to handle things like refreshing tokens and etc.
	*/
	class RateTracker
	{
	public:

		/*
		* This constructor takes the max amound of acceptable requests per minute, and it divides it by 
		* 60,000 to get the time difference between each requests in milliseconds
		*/
		RateTracker(const int max_rate_minute_limit, std::optional<int> max_time = std::nullopt);

		/*
		* This sends the request and makes sure it doesn't exceed any limits. And if the request fails with a code 429,
		* it will try again but with a longer delay each attempt.
		*/
		HttpResponse Send(const URI& srcUri, const HttpRequest& req);

		/*
		* Set the maxumum ammount of tries to retry after a request fails
		*/
		inline void SetMaxTries(int n)
		{
			MaxTries = n;
		}

		inline const int GetMaxTries()
		{
			return MaxTries;
		}

		inline void SetMaxTime(std::chrono::seconds time)
		{
			this->MaxTime.emplace(time);
		}
		
		// Start tracking time 
		inline void Track()
		{
			if(MaxTime)
				StartTime.emplace(std::chrono::system_clock::now());
		}

	private:
		// A temporary storage to keep our intervals
		std::deque<Millisecond> Cache;
		Millisecond minimum_time_interval{ 0 };
		int MaxTries{ 5 };
		int Tries{ 0 };

		std::optional<std::chrono::seconds> MaxTime{ std::nullopt };
		std::optional<TimePoint> StartTime{ std::nullopt };
		std::mutex mlock;

		bool TimeUp();

		inline Millisecond Now() {
			return std::chrono::duration_cast<Millisecond>(std::chrono::system_clock::now().time_since_epoch());
		}

		/*
		* Returns the difference of time between now and the most recent time from Cache in milliseconds, else if 
		* Cache is empty return 0 milliseconds
		*/
		inline Millisecond Delta()
		{
			std::lock_guard<std::mutex> guard(mlock);
			return Cache.empty() ? Millisecond(0) : Millisecond(Now().count() - Cache[0].count());
		}

		/*
		* Returns an interval that break the minimum time interval, or is lower than it, and returns it in milliseconds.
		*/
		inline Millisecond GetLatestInterval()
		{
			std::lock_guard<std::mutex> guard(mlock);
			UpdateCache();
			return (minimum_time_interval > Delta()) ? (minimum_time_interval - Delta()) : Millisecond(0);
		}

		/*
		* Removes any old data that doesn't break limits
		*/
		void UpdateCache();

		inline void UpdateCache(const Millisecond m)
		{
			std::lock_guard<std::mutex> guard(mlock);
			Cache.emplace_back(m);
		}

		/*
		* Waits the amount of time if an interval is lower than the minimum, and this goes up the number of requests done represented by n
		*/
		void waitifnecessary(int n);

	};
};
