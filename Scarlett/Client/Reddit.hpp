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
		template<Client U>
		friend 	RedditAuthorization<U>& operator<<(const RedditScope rs, RedditAuthorization<U>& au);
		std::string ClientId, Secret, RedirectURI, Username, Password;

		std::shared_ptr<HttpClient::http_client> client;
		std::shared_ptr<HttpClient::http_client_config> clientConfig;
		void InitConnection();
		void InitAccessData(struct AccessData& ad);
		std::vector<RedditScope> scopes;
		std::string GenerateScope();
	};

	template<Client C>
	RedditAuthorization<C>& operator<<(const RedditScope rs, RedditAuthorization<C>& au)
	{
		au.scopes.push_back(rs);
		return au;
	}


	template<Client C>
	class RedditClient : public RedditAuthorization<C>
	{
	public:
		RedditClient(const struct AccessData& ad);
		//std::enable_if_t< std::is_same(C, Client::Implicit), bool>::type Authorize();
	private:
		std::shared_ptr<HttpClient::http_client> client;
	};

	template<Client C>
	inline RedditClient<C>::RedditClient(const AccessData& ad) : RedditAuthorization<C>(ad)
	{
		client = RedditAuthorization<C>::getClient();
	}
};
