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
};
