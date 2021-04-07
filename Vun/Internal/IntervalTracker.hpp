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
namespace Vun::Internal
{
	
	using HttpResponse = web::http::http_response;
	using HttpRequest = web::http::http_request;
	using HttpClient = web::http::client::http_client;
	using Millisecond = std::chrono::milliseconds;
	class RateTracker
	{
	public:
		RateTracker(int max_rate_minute_limit, std::optional<int> TotalTime = std::nullopt);

		
		int TotalRequests{ 0 };

		HttpResponse Send(const std::string& URL, const HttpRequest& req);

	private:
		std::deque<Millisecond> Cache;
		std::chrono::milliseconds max_interval_time{ 0 };
		std::optional<std::chrono::seconds> MaxTime;

		inline Millisecond now() {
			return std::chrono::duration_cast<Millisecond>(std::chrono::system_clock::now().time_since_epoch());
		}

		inline Millisecond delta()
		{
			return Cache.empty() ? Millisecond(0) : Millisecond(now().count() - Cache[0].count());
		}

		inline Millisecond latestInterval()
		{
			UpdateCache();
			return (max_interval_time > delta()) ? (max_interval_time - delta()) : Millisecond(0);
		}

		void UpdateCache();
		void waitifnecessary(int n);

	};
};
