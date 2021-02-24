#pragma once 

#include <ctime>
#include <string>
#include <vector>

namespace Vun::Reddit
{
    enum class AuthMethod
	{
        Implicit,
        Script,
        Userless
        UserlessScript
	};
    
    enum RedditScope
	{
		Identity = 0,
		Edit,
		Flair,
		History,
		ModConfig,
		ModFlair,
		ModLog,
		ModPosts,
		ModWiki,
		MySubs,
		PrivateMessages,
		Read,
		Report,
		Save,
		Submit,
		Subscribe,
		Vote,
		WikiEdit,
		WikiRead
	};
    
	static constexpr std::string_view stringScopes[] = {
		   "identity",
		   "edit",
		   "flair",
		   "history",
		   "modconfig",
		   "modflair",
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

    

    template<Client C>
    struct AccessData
    {
        std::string ClientId, RedirectURI, UserAgent;
        
        std::enable_if_t< std::is_same<C, Client::Script>::value, std::string>::type
        Secret, Username, Password;
        
        std::enable_if_t< std::is_same<C, Client::Userless>::value, std::string>::type
        DeviceId;
        
        std::enable_if_t< std::is_same<C, Client::Userless>::value || std::is_same<C, Client::UserlessScript>, bool>::type
        IsPermanent;
        
        std::enable_if_t< std::is_same<C, Client::Userless>::value || std::is_same<C, Client::UserlessScript>, std::string>::type
        RefreshToken;
        
        
        
        
        std::vector<RedditScope> scopes;
        
        std::string BearerCode, AccessToken;
        std::string getScope();
    };
    
}
