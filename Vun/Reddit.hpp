#pragma once

#include <string>
#include <algorithm>
#include <random>
#include <memory>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <string_view>

namespace HttpClient = web::http::client;
namespace OAuth2 = web::http::oauth2::experimental;
namespace conv = utility::conversions;

namespace Vun::Reddit
{
	enum class RedditScope
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

	enum class Client
	{
		Code,
		Implicit,
		CC,
		PSW
	};

	struct AccessData
	{
		std::string ClientId, Secret, RedirectURI, UserAgent, Username, Password;
	};


	template<Client C>
	class RedditAuthorization
	{
	public:
		RedditAuthorization(const struct AccessData& ad);
		inline std::shared_ptr<HttpClient::http_client> getClient()
		{
			return client;
		}
	private:
		friend 	RedditAuthorization<C>& operator<<(const RedditScope rs, RedditAuthorization<C> au);
		std::string ClientId, Secret, RedirectURI, Username, Password;

		std::shared_ptr<HttpClient::http_client> client;
		HttpClient::http_client_config clientConfig;
		void InitConnection();
		std::vector<RedditScope> scopes;
		std::string GenerateScope();
	};

	template<Client C>
	RedditAuthorization<C>& operator<<(const RedditScope rs, RedditAuthorization<C> au)
	{
		au.scopes.push_back(rs);
		return au;
	}
};
