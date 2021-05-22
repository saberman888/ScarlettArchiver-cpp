#pragma once

#include <string>
#include <algorithm>
#include <random>
#include <memory>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <string_view>
#include "OAuth2.hpp"

namespace HttpClient = web::http::client;
namespace OAuth2 = web::http::oauth2::experimental;
namespace conv = utility::conversions;

namespace Scarlett::Client
{
	enum RedditScope
	{
		identity = 0,
		edit,
		flair,
		history,
		modconfig,
		modlog,
		modposts,
		modwiki,
		mysubreddits,
		privatemessages,
		read,
		report,
		save,
		submit,
		subscribe,
		vote,
		wikiedit,
		wikiread
	};

	struct AccessData
	{
		std::string client_key, client_secret, useragent, redirect_uri;
		std::string username, password;
	};


	template<typename T>
	class RedditClient : private OAuth2Helper<T>
	{
	public:
		RedditClient(struct AccessData& acd) : OAuth2Helper<T>(""_u,""_u,""_u,""_u)
		{
			if constexpr (std::is_same<T, _Password>::value)
			{
				this->setUserCredentials(u16(acd.username), u16(acd.password));
				this->setClientSecret(u16(acd.client_key));
				this->setClientSecret(u16(acd.client_secret));
				this->setUserAgent(u16(acd.useragent));
			}
		}

		void AuthorizeWithReddit()
		{
			try {
				this->GetToken();
			}
			catch (std::exception& e) {
				scarlettNestedThrow(e.what());
			}
		}
	};

	using SimpleClient = RedditClient<_Password>;
	using AuthClient = RedditClient<Authorization>;

};
