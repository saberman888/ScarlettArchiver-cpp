#include <iostream>
#include "../scarlett_pch.hpp"


namespace Scarlett::Tests
{
	TEST_F(TestAccessData, Authorization)
	{
		using namespace Scarlett::Client;
		auto acd = TestAccessData::load_configuration("accessdata.xml"); 
		//InitLogs();
		RedditClient<Authorization> authGrant(acd);
		authGrant << reddit_scopes::identity << reddit_scopes::account << reddit_scopes::history;
		ASSERT_NO_THROW(authGrant.AuthorizeWithReddit());
		ASSERT_NO_THROW(authGrant.Me());
	}
}
