#include "Scarlett/Client/Reddit.hpp"
#include "Scarlett/Client/OAuth2.hpp"


namespace Scarlett::Client
{




	template<typename T>
	class RedditClient<T>::Impl
	{
	public:
		std::shared_ptr<OAuth2Authorization> oauth2handle;
		std::vector<reddit_scope> scopes;


		Impl(const struct AccessData& acd)
		{
			oauth2handle = std::make_shared<OAuth2Authorization>(acd);
		}

		Impl(const String client_key, const String client_secret, const String redirect_uri, const String useragent)
		{
			oauth2handle = std::make_shared<OAuth2Authorization>(client_key, client_secret, redirect_uri, useragent);
		}

		Impl(Impl& other) = default;
		Impl(Impl&& other) = default;

		~Impl() = default;

		bool AuthorizeWithReddit()
		{
			if constexpr (std::is_same_v<T, PasswordGrant>)
			{
				return oauth2handle->RequestTokenFromPassword();
			}
			else {
				return oauth2handle->RequestTokenFromCode();
			}
			return false;
		}


		void GenerateScopes()
		{
			String scope_string;
			if (scopes.size() != 0)
			{

				for (auto& scope : scopes)
				{
					scope_string += scope;
					scope_string += " "_u;
				}
			}
			else {
				scope_string = "*"_u;
			}
			oauth2handle->setScope(scope_string);
		}
		// TODO: Implement GET and POST functions that take the oauth2 tokens
		/*auto RedditGet(const std::string& endpoint, const std::optional<StringMap> query_parameters)
		{
			web::uri_builder ub;
			ub.set_scheme("https"_u);
			ub.set_host("api.reddit.com"_u);
			ub.set_path(toScarlettString(endpoint));
			if (query_parameters) {
				ub.append_query(toScarlettString(GenerateParamData(query_parameters.value())));
			}
			auto r = HttpRequest(web::http::methods::GET);
			r.set_request_uri(ub.to_uri());
			return oauth2handle->Send(ub.to_uri(), r);
		}*/


	};

#define _SCARLETT_REDDIT_SCOPE
#define DAT(a_, b_) const reddit_scope reddit_scopes::a_(SCARLETT_WIDEN(b_));
#include "Scarlett/Internal/ScarlettConstants.h"
#undef _SCARLETT_REDDIT_SCOPE
#undef DAT

	template<typename T>
	RedditClient<T>::RedditClient(const struct AccessData& acd)
	{
		impl = std::make_unique<Impl>(acd);
	}

	template<typename T>
	RedditClient<T>::RedditClient(const String client_key, const String client_secret, const String redirect_uri, const String useragent)
	{
		impl = std::make_unique<Impl>(client_key, client_secret, redirect_uri, useragent);
	}

	template<typename T>
	RedditClient<T>::RedditClient(const String username, const String password, const String client_key, const String client_secret, const String redirect_uri, const String useragent)
	{
		impl = std::make_unique<Impl>(client_key, client_secret, redirect_uri, useragent);
		impl->oauth2handle->setUserCredentials(username, password);
	}

	template<typename T>
	RedditClient<T>::RedditClient(RedditClient&& other) = default;

	template<typename T>
	RedditClient<T>::RedditClient(const RedditClient& other)
	{
		impl = std::make_unique<Impl>(*other.impl);
	}

	template<typename T>
	RedditClient<T>& RedditClient<T>::operator=(const RedditClient& other)
	{
		impl = std::make_unique<Impl>(*other.impl);
		return *this;
	}

	template<typename T>
	RedditClient<T>::~RedditClient() = default;

	template<typename T>
	bool RedditClient<T>::AuthorizeWithReddit()
	{
		return impl->AuthorizeWithReddit();
	}

	template<typename T>
	RedditClient<T>& RedditClient<T>::operator<<(const reddit_scope& scope)
	{
		impl->scopes.push_back(scope);
		return *this;
	}
	template class RedditClient<PasswordGrant>;
	template class RedditClient<AuthorizationGrant>;
}
