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
namespace Scarlett
{
	using HttpResponse = web::http::http_response;
	using HttpRequest = web::http::http_request;
	using HttpClient = web::http::client::http_client;
	using Millisecond = std::chrono::milliseconds;
	using HttpMethod = web::http::methods;
	using URI = web::uri;

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
		RateTracker(int max_rate_minute_limit);

		/*
		* This sends the request and makes sure it doesn't exceed any limits. And if the request fails with a code 429,
		* it will try again but with a longer delay each attempt.
		*/
		HttpResponse Send(const URI& srcUri, const HttpRequest& req);

		/*
		* Set the maxumum ammount of tries to retry after a request fails
		*/
		constexpr inline void SetMaxTries(int n)
		{
			MaxTries = n;
		}

	private:
		// A temporary storage to keep our intervals
		std::deque<Millisecond> Cache;
		Millisecond minimum_time_interval{ 0 };
		int MaxTries{ 5 };

		inline Millisecond Now() {
			return std::chrono::duration_cast<Millisecond>(std::chrono::system_clock::now().time_since_epoch());
		}

		/*
		* Returns the difference of time between now and the most recent time from Cache in milliseconds, else if 
		* Cache is empty return 0 milliseconds
		*/
		inline Millisecond Delta()
		{
			return Cache.empty() ? Millisecond(0) : Millisecond(Now().count() - Cache[0].count());
		}

		/*
		* Returns an interval that break the minimum time interval, or is lower than it, and returns it in milliseconds.
		*/
		inline Millisecond GetLatestInterval()
		{
			UpdateCache();
			return (minimum_time_interval > Delta()) ? (minimum_time_interval - Delta()) : Millisecond(0);
		}

		/*
		* Removes any old or data that doesn't break limits
		*/
		void UpdateCache();

		/*
		* Waits the amount of time if an interval is lower than the minimum, and this goes up the number of requests done represented by n
		*/
		void waitifnecessary(int n);

	};
};
