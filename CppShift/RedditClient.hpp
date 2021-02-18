#pragma once

#include <string>
#include <vector>
#include <array>
#include <sstream>
#include "BasicRequest.hpp"
#include "nlohmann/json.hpp"
namespace Reddit {
	enum Scopes
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

	enum class Client
	{
		Script,
		Compact,
		Implicit,
		Userless
	};

	enum class Duration
	{
		Temporary,
		TemporaryWithoutRefresh,
		Permanent
	};

	template<Client C>
	class AppInfo
	{
	public:
		friend AppInfo& operator<<(AppInfo& ai, const Scopes s);
		AppInfo(const std::string Id, const std::string UserAgent, const std::string RedirectURI = "localhost:8080", const Duration dt = Temporary);
		AppInfo(const std::string Id, const std::string Secret, const std::string UserAgent, const std::string RedirectURI = "localhost:8080", const Duration dt = Temporary);
		void setUserCredentials(const std::string Username, const std::string Password);
		inline bool Authorized();

		bool RevokeAccess();
		inline bool Revokable();
		void DisableRevoke();
	private:
		bool Perform();
		const std::string Id, RedirectURI, Secret, ResponseCode;
		const std::string Username, Password, UserAgent;
		Duration dt;
		bool IsAuthorized, IsRevokable;
		std::vector<Scopes> Scope;
		const inline static std::array<std::string, 20> stringScope {
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
	};

	using ScriptClient = AppInfo<Client::Script>;
	using AppClient = AppInfo<Client::Implicit>;
	using UserlessClient = AppInfo<Client::Userless>;
	using CompactClient = AppInfo<Client::Compact>;

	template<Client C>
	AppInfo<C>& operator<<(AppInfo<C>& ai, const Scopes s)
	{
		ai.Scope.push_back(s);
		return ai;
	}


	template<Client C>
	inline AppInfo<C>::AppInfo(const std::string Id, const std::string UserAgent, const std::string RedirectURI, const Duration dt) : Id(Id), UserAgent(UserAgent), RedirectURI(RedirectURI), dt(dt)
	{}

	template<Client C>
	inline AppInfo<C>::AppInfo(const std::string Id, const std::string Secret, const std::string UserAgent, const std::string RedirectURI, const Duration dt) : Id(Id), Secret(Secret), UserAgent(UserAgent), RedirectURI(RedirectURI), dt(dt)
	{}

	template<Client C>
	inline void AppInfo<C>::setUserCredentials(const std::string Username, const std::string Password)	{
		this->Username = Username;
		this->Password = Password;
	}

	template<Client C>
	inline bool AppInfo<C>::Authorized()
	{
		return IsAuthorized;
	}

	template<Client C>
	inline bool AppInfo<C>::RevokeAccess()
	{
		// TODO: Implement revocation
		return false;
	}

	template<Client C>
	inline bool AppInfo<C>::Revokable()
	{
		return IsRevokable;
	}

	template<Client C>
	inline void AppInfo<C>::DisableRevoke()
	{
		//TODO: Implement disable revocation
	}

	template<Client C>
	inline bool AppInfo<C>::Perform()
	{

	}

	template<>
	inline bool AppInfo<Client::Userless>::Perform()
	{
		auto uniqueId = BasicRequest::UTCToString(0, "%Y%m%d%H%M%S");
		BasicRequest handle;
		handle.Setup("https://www.reddit.com/api/v1/access_token", true);
		handle.SetURLParameters({
			{"grant_type", "https://oauth.reddit.com/grants/installed_client&\device_id=" + uniqueId}
			});
		auto result = handle.SendRequest();
		return false;
	}

	template<>
	inline bool AppInfo<Client::Script>::Perform()
	{
		BasicRequest handle;
		handle.Setup("https://oauth.reddit.com/api/v1/access_token", true);
		handle.SetURLParameters({
			{"grant_type", "password"},
			{"username", Username},
			{"password", Password}
			});
		handle.SetCreds(Id + ":" + Secret);
		auto result = handle.SendRequest();
		if (!result.AllGood())
		{
			return false;
		}
		else {
			return true;
		}
	}

}
