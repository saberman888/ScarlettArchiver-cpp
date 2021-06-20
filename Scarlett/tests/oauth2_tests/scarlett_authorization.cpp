#include <iostream>
#include "../scarlett_pch.hpp"


namespace Scarlett::Tests
{
	TEST_F(TestAccessData, ImplicitGrant)
	{
		using namespace Scarlett::Client;
		OAuth2Helper<Authorization> a2h(
				acd.client_key,
				acd.client_secret,
				acd.redirect_uri,
				acd.useragent
				);
		a2h.setScope("*");
		ASSERT_TRUE(a2h.Authorize().get());
	}
}
