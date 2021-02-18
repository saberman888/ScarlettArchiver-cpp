#pragma once

#include <string>
#include <vector>
#include <array>
#include <sstream>

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
		AppInfo(const std::string Id, const std::string Secret, const std::string UserAgent, const std::string RedirectURL = "localhost:8080", const Duration dt = Temporary);
		inline bool Authorized();

		bool RevokeAccess();
		inline bool Revokable();
		void DisableRevoke();
	private:
		std::string Id, RedirectURI, Secret, ResponseCode;
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

	using ScriptClient = AppInfo<Script>;
	using AppClient = AppInfo<Implicit>;
	using UserlessClient = AppInfo<Userless>;

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
	inline AppInfo<C>::AppInfo(const std::string Id, const std::string Secret, const std::string UserAgent, const std::string RedirectURL, const Duration dt) : Id(Id), Secret(Secret), UserAgent(UserAgent), RedirectURL(RedirectURI), dt(dt)
	{
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

}
