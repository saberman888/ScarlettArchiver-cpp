#include "Scarlett/Reddit/BaseTypes/Listing.hpp"
#include <optional>
#include <type_traits>
#include <functional>
#include <boost/serialization/shared_ptr.hpp>
#include "Scarlett/Internal/Exceptions.hpp"
#include "Scarlett/Internal/Helpers.hpp"
#include "Scarlett/Internal/Serializable.hpp"

namespace Scarlett::Reddit
{
	template<typename T, typename U>
	class Listing<T, U>::ListingImpl
	{
	public:
		std::vector< boost::shared_ptr<T> > items;
		struct RedditStatistics statistics;

		std::function<bool(Listing<T, U>&)> HasNext;
		std::function<URI(const URI)> NextUpdate;

		bool Next()
		{
			if(HasNext(*this))
			{
				NextItems();
				NextUpdate();
				return true;
			} 
			return false;
		}

		void Clear()
		{
			items.clear();
			items.shrink_to_fit();
		}

		void NextItems(const StringMap& uri_query)
		{
			if constexpr (std::is_same_v<U, Client::PushShift>)
			{
				if constexpr (std::is_same_v<T, Thing>)
				{
					// If T is a non-comment
					auto response = Client::PushShift::SearchSubmissions(uri_query);
					if (response.status_code() != 200)
					{
						scarlettHTTPThrow(response);
					}
					else {
						Read(
							response.extract_json().get()
						);
					}

				}
				else if constexpr (std::is_same_v<T, Comment>) {
					// if T is a non-post or comment
				}
			}
		}

		void Add(boost::shared_ptr<T> Post)
		{
			statistics.Append<T>();
			if (items.size() > 0)
			{
				for (auto it = items.cbegin(); it != items.cend(); it++)
				{
					if (Post->getCreatedUTCTime() > (*it)->getCreatedUTCTime())
					{
						items.emplace(it, Post);
						return;
					}
				}
				items.push_back(Post);
			}
			else {
				items.push_back(Post);
			}
		}

		void Read(const JsonValue& source)
		{
			for (auto element : source.at("data"_u).as_array())
			{
				if (Gallery::IsGallery(element))
				{
					auto potentialPost = boost::make_shared<Gallery>(element);
					Add(potentialPost);
				}
				else if (Reddit::Video::IsVideo(element)) {
					auto potentialPost = boost::make_shared<Video>(element);
					Add(potentialPost);
				}
				else if (Reddit::SelfPost::IsSelfPost(element)) {
					auto potentialPost = boost::make_shared<SelfPost>(element);
					Add(potentialPost);
				}
				else {
					auto potentialPost = boost::make_shared<Link>(element);
					Add(potentialPost);
				}
			}
		}


	};


	template<typename T, typename U>
	Listing::Listing() = default;

	template<typename T, typename U>
	Listing::Listing(const std::string& endpoint)
	{
		impl = std::make_unique<T, U>();
	}

	template<typename T, typename U>
	Listing::Listing(const std::string& endpoint, const StringMap& query_params, std::function<URI(const URI)> nextupdate, std::function<bool(const Listing<T, U>&)> hasnext)
	{
		impl = std::make_unique<T, U>();
		impl->HasNext = hasnext;
		impl->NextUpdate = next;
	}
	
	template<typename T, typename U>
	Listing::~Listing() = default;

	template<typename T, typename U>
	bool Liusting::Next() { return impl->Next(); }

	template<typename T, typename U>
	const auto Listing::Posts() noexcept { return impl->items; }

	template<typename T, typename U>
	const auto Listing::Statistics() const noexcept { return impl->Statistics; }

	template<typename T, typename U>
	void Listing::Clear()
	{
		items.clear();
		items.shrink_to_fit();
	}

	template<typename T, typename U>
	const Size Listing::Size() const noexcept
	{
		return impl->items.size();
	}



}
