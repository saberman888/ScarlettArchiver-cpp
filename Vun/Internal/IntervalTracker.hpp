#pragma once

#include <chrono>
#include <optional>
#include <functional>

namespace Vun::Internal
{
	class IntervalTracker
	{
	public:
		IntervalTracker(std::optional<int> max_rate_minute_limit = std::nullopt);

		std::chrono::time_point<std::chrono::milliseconds> lastPoint;


		template<class T, class... Args>
		T intervene(std::function<T(Args...)> request)
		{
			auto t1 = std::chrono::system_clock::now();
			T result = request();
			lastPoint = (std::chrono::system_clock::now() - t1);
		}
		
	};
};