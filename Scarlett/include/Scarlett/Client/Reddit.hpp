#pragma once

#include <string>
#include <memory>
#include <string_view>
#include "../Internal/Types.hpp"
#include "AccessData.hpp"

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

	// TODO: Implement methods take use the API

	class PasswordGrant;
	class AuthorizationGrant;

	template<typename T>
	class SCDLL RedditClient
	{
	public:
		RedditClient(const struct AccessData& acd);
		RedditClient(RedditClient& other);
		RedditClient(RedditClient&& other);
		RedditClient(const String client_key, const String client_secret, const String redirect_uri, const String useragent);
		RedditClient(const String username, const String password, const String client_key, const String client_secret, const String redirect_uri, const String useragent);
		~RedditClient() = default;

		bool AuthorizeWithReddit();

		RedditClient& operator<<(const reddit_scope& scope);
	private:
		class _rcImpl;
		std::shared_ptr<_rcImpl> impl;
	};
	using AuthClient = RedditClient<AuthorizationGrant>;
	using SimpleClient = RedditClient<PasswordGrant>;

};
