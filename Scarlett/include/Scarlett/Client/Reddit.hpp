#pragma once

#include <string>
#include <algorithm>
#include <random>
#include <memory>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <string_view>
#include "OAuth2.hpp"
#include <fstream>
#include "../Internal/Types.hpp"

namespace Scarlett::Client
{
	using reddit_scope = String;
    class SCDLL reddit_scopes
	{
	public:
#define _SCARLETT_REDDIT_SCOPE
#define DAT(_a, _b) static const reddit_scope _a;
#include "../Internal/ScarlettConstants.h"
#undef _SCARLETT_REDDIT_SCOPE
#undef DAT
	};


	class PasswordGrant, AuthorizationGrant;

	template<typename T>
	class RedditClient
	{
	public:
		RedditClient(struct AccessData& acd)
		{

			oauth2handle = std::make_shared< OAuth2Authorization<T> >(
				acd.username, 
				acd.password, 
				acd.client_key, 
				acd.client_secret, 
				acd.redirect_uri, 
				acd.useragent);

			oauth2handle->SetMaxTime(
				std::chrono::seconds(3600)
			);
		}

		void Me()
		{
			auto v = RedditGet("/api/v1/me", StringMap{});
			auto r = v.extract_string().get();
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
			String scope_string;
			if(scopes.size() != 0)
			{

				for(auto& scope : scopes)
				{
					scope_string += scope;
					scope_string += " "_u;
				}
			} else {
				scope_string = "*"_u;
			}
			oauth2handle->setScope(scope_string);
		}

		auto RedditGet(const std::string& endpoint, const std::optional<StringMap> query_parameters)
		{
			web::uri_builder ub;
			ub.set_scheme("https"_u);
			ub.set_host("api.reddit.com"_u);
			ub.set_path(toScarlettString(endpoint));
			if(query_parameters){
				ub.append_query(toScarlettString(GenerateParamData(query_parameters.value())));
			}
			auto r = HttpRequest(web::http::methods::GET);
			r.set_request_uri(ub.to_uri());
			return oauth2handle->Send(ub.to_uri(), r);
		}

		std::shared_ptr< OAuth2Authorization > oauth2handle;
	};
	using AuthClient = RedditClient<AuthorizationGrant>;
	using SimpleClient = RedditClient<PasswordGrant>;

};
