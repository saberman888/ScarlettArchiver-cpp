#pragma once

#include <chrono>
#include <optional>
#include <functional>
#include <deque>
#include <tuple>
#include <cpprest/http_client.h>
#include <thread>
#include <algorithm>
namespace Vun::Internal
{
	
	using HTTPResponse = web::http::http_response;
	using HttpRequest = web::http::http_request;
	using HttpClient = web::http::client::http_client;
	using Millisecond = std::chrono::milliseconds;
	class BasicClient
	{
	public:
		BasicClient(int  max_rate_minute_limit, std::optional<int> TotalTime = std::nullopt);
		BasicClient(int MaxRequests, std::chrono::seconds TotalTime);

		std::deque<Millisecond> Cache;
		std::chrono::milliseconds max_interval_time;
		std::optional<std::chrono::seconds> MaxTime;
		int TotalRequests;

		HttpResponse Send(const std::string& URL, const HttpRequest& req);
		void checkTime();

		inline std::chrono::milliseconds updateTimes()
		{

			return std::chrono::system_clock::now();
		}

		inline Millisecond delta()
		{
			return Cache.empty()? 0 :  now() - Cache[0];
		}

		inline Millisecond latestInterval()
		{
			UpdateCache();
			return (max_interval_time > delta())? (max_interval_secs - delta()) : 0;
		}

		void UpdateCache();
		void waitifnecessary();

		

		template<class T, class... Args>
		T exec(std::function<T(Args...)> request)
		{
			while(true)
			{
				auto t1 = std::chrono::system_clock::now();
				T result = request();
				auto timePoint = (std::chrono::system_clock::now() - t1);
			}

			deque.push_front(lastPoint);
			return result;
		}
	}
};
