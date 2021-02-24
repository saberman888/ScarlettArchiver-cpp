#include "RedditClient.hpp"

namespace Vun::Reddit 
{
    template<Client C>
    RedditAccess<C>::RedditAccess(const std::string& ClientId, const std::string Secret, const std::string& UserAgent)
    {
        adata.ClientId = ClientId;
        adata.Secret = Secret;
        adata.UserAgent = UserAgent;
        InitConnection();
    }

    template<Client C>
	RedditAccess<C>::~RedditAccess()
	{
		delete tconn;
	}

	template<Client C>
	void RedditAccess<C>::setUserCredentials(const std::string& Username, const std::string& Password)
	{
		this->Username = Username;
		this->Password = Password;
	}
    
    template<Client C>
	bool RedditAccess::IsAuthorized()
	{
		return _IsAuthorizedWithReddit;
	}

	template<Client C>
	bool RedditAccess<C>::Perform()
	{
		RestClient::Response result = tconn->post("/api/v1/access_token", getGrant());
		return false;
	}
	
	template<Client C>
	std::string RedditAccess::getGrant()
    { 
        nlohmann::json formdata = {
            {"grant_type", "password"},
            {"username", adata.Username},
            {"password", adata.Password}
        };
        return formdata.dump();
    }
	
	template<>
	std::string Reddit<Userless>::getGrant()
    {
        nlohmann::json formdata = {
            {"grant_type", "https://oauth.reddit.com/grants/installed_client"},
            {"device_id", adata.DeviceId}
        };
        return formdata.dump();
    }
    
    template<>
	std::string Reddit<UserlessScript>::getGrant()
    {
        nlohmann::json formdata = {
            {"grant_type", "client_credentials"}
        };
        return formdata.dump();
    }
	
    
    template<Client C>
	void RedditAccess<C>::InitConnection()
	{
		tconn = new RestClient::Connection("https://www.reddit.com/");
		tconn->SetTimeout(5);
		tconn->FollowRedirects(true, 5);
		tconn->SetUserAgent(UserAgent);
		//tconn->SetBasicAuth(adata.Username, adata.Password);
	}

}
