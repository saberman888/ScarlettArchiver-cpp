#pragma once

#include "../../Client/OAuth2.hpp"
#include "../../Client/PushShift.hpp"
#include "../RedditStatistics.hpp"
#include "../../Internal/Types.hpp"
namespace Scarlett::Reddit
{
	template<typename T, typename U = Client::PushShift>
	class Listing
	{
	public:
		
		~Listing();

		bool Next();
		const auto Posts() const noexcept;

		const RedditStatistics Statistics() const noexcept;

		void Clear();

		const size_t Size() const noexcept;

	private:
		Listing();
		Listing(const std::string& endpoint);
		Listing(const std::string& endpoint, const StringMap& query_params, std::function<URI(const URI)> next, std::function<bool(const Listing<T, U>&)> hasnext);

		class ListingImpl;
		std::unique_ptr<ListingImpl> impl;
	};
};
