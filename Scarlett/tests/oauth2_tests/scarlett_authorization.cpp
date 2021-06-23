#include <iostream>
#include "../scarlett_pch.hpp"


namespace Scarlett::Tests
{
	TEST_F(TestAccessData, Authorization)
	{
		using namespace Scarlett::Client;
		auto acd = TestAccessData::load_configuration("accessdata.xml"); 
		RedditClient<Authorization> authGrant(acd);
		ASSERT_NO_THROW(authGrant.AuthorizeWithReddit());
	}
}
