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
	using reddit_scope = WideString;
	class reddit_scopes
	{
	public:
#define _REDDITSCOPE
#define DAT(_a, _b) static const reddit_scope _a;
#include "../Internal/ScarlettConstants.h"
#undef _REDDITSCOPE
#undef DAT
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
				oauth2handle->SetMaxTime(
					std::chrono::seconds(3600)
				);
			}
		}

		void Me()
		{
			auto request = HttpRequest(web::http::methods::GET);
			auto i = oauth2handle->Send("https://api.reddit.com/api/v1/me", request);
			auto v = i.extract_string().get();
		}

		void AuthorizeWithReddit()
		{
			try {
				GenerateScopes();
				oauth2handle->GetToken();
				oauth2handle->Track();
			}
			catch (std::exception& e) {
				scarlettNestedThrow(e.what());
			}
		}



		RedditClient<T>& operator<<(const reddit_scope& scope)
		{
			scopes.push_back(scope);
			return *this;
		}


	private:
		std::vector<reddit_scope> scopes;
		void GenerateScopes()
		{
			std::string scope_string;
			for(auto& scope : scopes)
			{
				scope_string += scope;
			}
			oauth2handle->setScope(scope_string);
		}

		std::shared_ptr< OAuth2Helper<T> > oauth2handle;
	};
	using AuthClient = RedditClient<Authorization>;

};
