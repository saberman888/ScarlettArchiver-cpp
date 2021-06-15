#include <iostream>
#include "../scarlett_pch.hpp"


namespace Scarlett::Tests
{
	TEST_F(TestAccessData, PasswordGrant)
	{
		using namespace Scarlett::Client;
		RedditClient<PasswordGrant> pswg(acd);

		ASSERT_NO_THROW(pswg.AuthorizeWithReddit());
		ASSERT_NO_THROW(pswg.Me());

	}

	TEST_F(TestAccessData, ImplicitGrant)
	{
		using namespace Scarlett::Client;
		OAuth2Helper<Authorization> a2h(
			u16(acd.username),
			u16(acd.password),
			u16(acd.client_key),
			u16(acd.client_secret),
			u16(acd.redirect_uri),
			u16(acd.useragent));
		ASSERT_TRUE(a2h.Authorize().get());
	}
};
