#pragma once

#include "../../Internal/Helpers.hpp"
#include "../../Internal/Serializable.hpp"
#include "../../Client/OAuth2.hpp"
#include "../../Client/PushShift.hpp"
#include "../RedditStatistics.hpp"
#include "../../Internal/Exceptions.hpp"
#include "../../Internal/Types.hpp"
#include <optional>
#include <type_traits>
#include <functional>
#include <boost/serialization/shared_ptr.hpp>
namespace Scarlett::Reddit
{
	template<typename T, typename U = Client::PushShift>
	class Listing
	{
	public:
		
		~Listing();

		void Next();
		bool HasNext();

		const auto Posts() const noexcept;

		const auto Statistics() const noexcept;

		void Clear();

		const Size Size() const noexcept;

	protected:


		void setEndpoint(const std::string& endpoint);

		void NextItems(const StringMap& uri_query);

		void NextItems(const std::string& endpoint, const StringMap& uri_query);

		void Add(boost::shared_ptr<T> Post);

	private:
		Listing();
		Listing(const std::string& endpoint);
		Listing(const std::string& endpoint, const StringMap& query_params, std::function<URI(const URI)> next, std::function<bool(const Listing<T, U>&)> hasnext);

		class ListingImpl;
		std::unique_ptr<ListingImpl> impl;
	};
};
