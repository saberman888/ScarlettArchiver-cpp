#include "Reddit.hpp"

namespace Vun::Reddit
{
	template<Client C>
	std::string RedditAuthorization<C>::GenerateRandomString()
	{
		/*auto randchar = []() -> char
		{
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		std::string str(length, 0);
		std::generate_n(str.begin(), length, randchar);*/
		return std::string();
	}

	template<Client C>
	std::string RedditAuthorization<C>::GenerateScope()
	{
		constexpr std::string_view stringScope[]
		{
			"identity",
			"edit",
			"flair",
			"history",
			"modconfig",
			"modlog",
			"modposts",
			"modwiki",
			"mysubreddits",
			"privatemessages",
			"read",
			"report",
			"save",
			"submit",
			"subscribe",
			"vote",
			"wikiedit",
			"wikiread"
		};
		std::string whole;
		for (auto scope : scopes)
		{
			whole += ("%20" + stringScope[scope]);
		}
		return whole;
	}


	template<Client C>
	RedditAuthorization<C>::RedditAuthorization(const AccessData ad)
	{
		ClientId = ad.ClientId;
		Secret = ad.Secret;
		RedirectURI = ad.RedirectURI;
		Username = ad.Username;
		Password = ad.Password;
		InitConnection();
	}

	template<Client C>
	void RedditAuthorization<C>::InitConnection()
	{
		auto oauthconfig = clientConfig.oauth2();

		if constexpr (std::is_same<C, Client::Implicit>)
			oauthconfig->set_implicit_grant(true);

		if constexpr (std::is_same<C, Client::Implicit> || std::is_same<C, Client::Code>)
		{
			oauthconfig->set_auth_endpoint(conv::to_string_t("/api/v1/authorize"));
			oauthconfig->set_redirect_uri(conv::to_string_t(RedirectURI));
			//oauthconfig->set_state(conv::to_string_t(GenerateRandomString()));
			oauthconfig->build_authorization_uri(true);
		}

		oauthconfig->set_client_key(conv::to_string_t(ClientId));
		
		oauthconfig->set_token_endpoint(conv::to_string_t("/api/v1/access_token"));
		oauthconfig->set_scope(conv::to_string_t(GenerateScope));
		oauthconfig->set_timeout(30);

		web::uri URL = web::uri("https://www.reddit.com/");

		client = std::make_shared<HttpClient::http_client>(URL, clientConfig);

	}

};