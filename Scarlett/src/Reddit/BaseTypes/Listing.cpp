#include "Scarlett/Reddit/BaseTypes/Listing.hpp"




/*namespace Scarlett
{
	namespace Internal
	{
		template<typename T, typename U>
		class ListingImplement
		{
		public:
			ListingImplement() = default;

			std::vector< boost::shared_ptr<T> > items;
			struct RedditStatistics statistics;

			bool Next()
			{
				return false;
			}


			const T Posts() const noexcept
			{
				return items;
			}

			const RedditStatistics Statistics() const noexcept
			{
				return statistics;
			}

			void Clear()
			{
				items.clear();
				items.shrink_to_fit();
			}

			template<typename W>
			void SetOAuth2Settings(std::shared_ptr<OAuth2Authorization<W>)

			Size Size() const noexcept
			{
				return items.size();
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

			void Read(const JSON::value& source)
			{
				using namespace Reddit;
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

			void NextItems(const std::string& endpoint, const StringMap& uri_query) {};
		};
	}
}*/