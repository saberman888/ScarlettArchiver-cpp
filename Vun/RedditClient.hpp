#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <sstream>
#include <mutex>
#include <cstdlib>
#include <memory>
#include <optional>
#include <nlohmann/json.hpp>
#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>
#include "AccessData.hpp"

namespace Vun::Reddit {


    template<Client C>
	class RedditAccess
	{
	public:
		RedditAccess(const std::string& ClientId, const std::string Secret, const std::string& UserAgent);
		~RedditAccess();
        
        bool IsAuthorized();
        
        std::enable_if_t<std::is_same<C, Client::Script>::value, bool>::type 
        

        
        std::enable_if_t<std::is_same<C, Client::Implicit>::value, bool>::type 
        AuthorizeWithUser();
        

        std::enable_if_t<std::is_same<C, Client::Implicit>::value, bool>::type
        IsLoggedIn();
        
		bool AuthorizeWithReddit();
        
        std::vector<Scope> Scopes();

	private:
        struct AccessData<C> adata;
        int Ratelimit-Used;
        int Ratelimit-Remaining;
        time_t Ratelimit-Reset;

		RestClient::Connection* tconn;
		void InitConnection();
        std::string getGrant();

	};
    
    template<Client C>
    RedditAccess<C>& operator<<(RedditAccess<C>& ra, const RedditScope rs);

};
