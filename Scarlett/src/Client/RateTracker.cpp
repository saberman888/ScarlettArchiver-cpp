#include "Scarlett/Client/RateTracker.hpp"
#include "Scarlett/Internal/Exceptions.hpp"
#include <optional>
#include <functional>
#include <deque>
#include <tuple>
#include <thread>
#include <algorithm>
#include <exception>
#include <mutex>
#include <chrono>
#include <cpprest/http_client.h>
namespace Scarlett::Client
{

	class RateTracker::_rtImpl
	{
	public:
		using Millisecond = std::chrono::milliseconds;

		_rtImpl(const MinuteRate mmr, std::optional<Second> max_time = std::nullopt)
		{
			minimum_time_interval = Millisecond(60000 / mmr);
		}

		// A temporary storage to keep our intervals
		std::deque<Millisecond> Cache;
		Millisecond minimum_time_interval{ 0 };
		int MaxTries{ 5 };
		int Tries{ 0 };

		std::optional<std::chrono::seconds> MaxTime{ std::nullopt };
		std::optional<TimePoint> StartTime{ std::nullopt };
		std::mutex mlock;

		// Start tracking a timepoint
		void Track()
		{
			if (MaxTime)
				StartTime.emplace(std::chrono::system_clock::now());
		}

		Millisecond Now() {
			return std::chrono::duration_cast<Millisecond>(std::chrono::system_clock::now().time_since_epoch());
		}

		bool TimeUp()
		{
			std::lock_guard<std::mutex> guard(mlock);
			if (!StartTime)
			{
				if (StartTime.value() >= (StartTime.value() + std::chrono::hours(1)))
				{
					return true;
				}
			}
			return false;
		}

		/*
		* Returns the difference of time between now and the most recent time from Cache in milliseconds, else if
		* Cache is empty return 0 milliseconds
		*/
		Millisecond Delta()
		{
			std::lock_guard<std::mutex> guard(mlock);
			return Cache.empty() ? Millisecond(0) : Millisecond(Now().count() - Cache[0].count());
		}

		/*
		* Removes any old data that doesn't break limits
		*/
		void UpdateCache()
		{
			std::lock_guard<std::mutex> guard(mlock);
			while (Delta() > minimum_time_interval)
			{
				Cache.pop_back();
			}
		}

		// Add new time to the cache
		void UpdateCache(const Millisecond m)
		{
			std::lock_guard<std::mutex> guard(mlock);
			Cache.emplace_back(m);
		}

		/*
		* Returns an interval that break the minimum time interval, or is lower than it, and returns it in milliseconds.
		*/
		Millisecond GetLatestInterval()
		{
			std::lock_guard<std::mutex> guard(mlock);
			UpdateCache();
			return (minimum_time_interval > Delta()) ? (minimum_time_interval - Delta()) : Millisecond(0);
		}

		/*
		* Waits the amount of time if an interval is lower than the minimum, and this goes up the number of requests done represented by n
		*/
		void waitifnecessary(int n)
		{
			// Get an interval if there's any
			Millisecond interval = GetLatestInterval();

			// Multiply the number of tries by 200ms and make sure it's under 1 hour
			Millisecond waitTimes = Millisecond(200 * n);
			waitTimes = waitTimes.count() < 3600 * 1000 ? waitTimes : Millisecond(3600 * 100);

			std::this_thread::sleep_for(interval);
			std::this_thread::sleep_for(waitTimes);

		}

		/*
		* This sends the request and makes sure it doesn't exceed any limits. And if the request fails with a code 429,
		* it will try again but with a longer delay each attempt.
		*/
		HttpResponse Send(const URI& srcUri, const HttpRequest& req)
		{
			web::http::client::http_client cl{ srcUri.authority().to_string() };
			HttpResponse hr;

			if (TimeUp())
			{
				scarlettOOTThrow("Error: Token's time has expired");
			}

			if (Tries >= MaxTries)
			{
				scarlettThrow("Error: Max number of requests reached");
			}

			bool complete = false;
			int tries = 0;
			while (!complete && tries < MaxTries)
			{
				// Depending on how many retries and how long the last attempt was wait if neccessary
				waitifnecessary(tries);

				// Next, initiate a request, time it, and push it into cache
				auto beginTime = Now();
				hr = cl.request(req).get();
				UpdateCache(Now() - beginTime);

				tries++;

				if (hr.status_code() != 429 || hr.status_code() == 200)
				{
					complete = true;
					break;
				}
			}

			return hr;
		}
	};


	RateTracker::RateTracker(const MinuteRate mmr)
	{
		impl = std::make_unique<_rtImpl>(mmr);
	}

	RateTracker::RateTracker(const MinuteRate mmr, const Second mht)
	{
		impl = std::make_unique<_rtImpl>(mmr, mht);
	}

	RateTracker::RateTracker(RateTracker& rt)
	{
		impl.reset(rt.impl.get());
	}

	void RateTracker::setMaxTries(int n)
	{
		impl->MaxTries = n;
	}

	int RateTracker::MaxTries()
	{
		return impl->MaxTries;
	}

	void RateTracker::setMaxTime(const Second time)
	{
		impl->MaxTime.emplace(time);
	}

	void RateTracker::Track()
	{
		impl->Track();
	}

	std::chrono::seconds RateTracker::MaxTime()
	{
		return impl->MaxTime.value();
	}

	HttpResponse RateTracker::Send(const URI& source, const HttpRequest& hr)
	{
		return impl->Send(source, hr);
	}

};
