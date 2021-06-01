#pragma once

#include "../../Internal/Helpers.hpp"
#include "../../Internal/Serializable.hpp"
#include "../../Client/OAuth2.hpp"
#include "../../Client/PushShift.hpp"
#include "../RedditStatistics.hpp"
#include <optional>
#include <type_traits>

namespace Scarlett::Reddit::BaseTypes
{
	template<typename T, typename U = Client::PushShift>
	class Listing
	{
	public:
		Listing() = default;

		virtual void Next() {};
		virtual bool HasNext() { return false; };

		inline const auto Posts()
		{
			return items;
		}

		inline const auto Statistics()
		{
			return statistics;
		}

	protected:
		void NextItems(const StringMap& uri_query)
		{
			if constexpr (std::is_same_v<U, Client::PushShift>)
			{
				if constexpr (std::is_same_v<T, Linkable>)
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
				else if constexpr(std::is_same_v<T, Comment>){
					// if T is a non-post or comment
				}
			}
		}

		void NextItems(const std::string& endpoint, const StringMap& uri_query) {};

		void Add(boost::shared_ptr<T> Post)
		{
			statistics.Append<T>();
			if (items.size() > 0)
			{
				for (decltype(items)::const_iterator it = items.cbegin(); it != items.cend(); it++)
				{
					// internal creation date
					auto icd = boost::dynamic_pointer_cast<BaseTypes::Postable>(*it);

					if (Post->CreatedUTC > icd->CreatedUTC)
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

	private:
		void Read(const JSON::value& source)
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
					auto potentialPost = boost::make_shared<BaseTypes::Link>(element);
					Add(potentialPost);
				}
			}
		}
		std::vector< boost::shared_ptr<T> > items;
		struct RedditStatistics statistics;
	};
};
