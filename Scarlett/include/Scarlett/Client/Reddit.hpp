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
	class RedditClient
	{
	public:
		RedditClient(struct AccessData& acd)
		{

			if constexpr (std::is_same<T, PasswordGrant>::value)
			{	
				oauth2handle = std::make_shared< OAuth2Helper<T> >(acd.username, acd.password, acd.client_key, acd.client_secret, acd.redirect_uri, acd.useragent);
				oauth2handle->SetMaxTime(3600);
			}
		}

		void AuthorizeWithReddit()
		{
			try {
				oauth2handle->GetToken();
			}
			catch (std::exception& e) {
				scarlettNestedThrow(e.what());
			}
		}
	private:
		std::shared_ptr< OAuth2Helper<T> > oauth2handle;
	};

	using SimpleClient = RedditClient<PasswordGrant>;
	using AuthClient = RedditClient<Authorization>;

};
