#include "Reddit.hpp"

namespace Scarlett::Client
{
#if 0
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
			whole += ("%20" + static_cast<std::string>(stringScope[scope]));
		}
		return whole;
	}


	template<Client C>
	RedditAuthorization<C>::RedditAuthorization(const struct AccessData& ad)
	{
		InitAccessData(ad);
		InitConnection();
	}

	template<Client C>
	void RedditAuthorization<C>::InitAccessData(struct AccessData& ad)
	{
		ClientId = ad.ClientId;
		Secret = ad.Secret;
		RedirectURI = ad.RedirectURI;
		Username = ad.Username;
		Password = ad.Password;
	}

	template<Client C>
	void RedditAuthorization<C>::InitConnection()
	{
		auto oauthconfig = clientConfig->oauth2();

		if (C == Client::Implicit)
			oauthconfig->set_implicit_grant(true);

		if(C == Client::Implicit || C == Client::Code)
		{
			oauthconfig->set_auth_endpoint(conv::to_string_t("/api/v1/authorize"));
			oauthconfig->set_redirect_uri(conv::to_string_t(RedirectURI));
			oauthconfig->build_authorization_uri(true);
		}
		else if(C == Client::PSW){
			oauthconfig->set_token_endpoint(conv::to_string_t("/api/v1/access_token"));
		}

		oauthconfig->set_client_key(conv::to_string_t(ClientId));
		oauthconfig->set_scope(conv::to_string_t(GenerateScope));
		clientConfig->set_timeout(30);
		

		clientConfig->set_oauth2(*oauthconfig.get());

		client = std::make_shared<HttpClient::http_client>(web::uri("https://www.reddit.com/", clientConfig));
	}
	#endif
};