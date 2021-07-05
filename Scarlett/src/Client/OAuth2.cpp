#include "Scarlett/Client/OAuth2.hpp"
#include "Scarlett/Client/RateTracker.hpp"
#include "cpprest/http_listener.h"
#include <mutex>
#include <utility>

namespace Scarlett::Client
{

	namespace Internal
	{
		class oauth2_code_listener
		{
		public:
			using oauth2_config = web::http::oauth2::experimental::oauth2_config;
			using http_listener = web::http::experimental::listener::http_listener;
			using http_request = web::http::http_request;
			using oauth2_exception = web::http::oauth2::experimental::oauth2_exception;
			using status_codes = web::http::status_codes;


			oauth2_code_listener(URI listen_uri, std::shared_ptr<oauth2_config>& config)
				: m_listener(new http_listener(listen_uri)), m_config(config)
			{
				if (!config)
					throw std::invalid_argument("Fatal Error: config has no value");
				m_listener->support([this](http_request request) -> void {
					if (request.request_uri().path() == m_listener->uri().path())
					{
						m_resplock.lock();

						m_config->token_from_redirected_uri(request.request_uri())
							.then([this, request](pplx::task<void> token_task) -> void {
							try
							{
								token_task.wait();
								m_tce.set(true);
							}
							catch (const oauth2_exception& e)
							{
								ucout << "Error: " << e.what() << std::endl;
								m_tce.set(false);
								scarlettNestedThrow("Fatal Error: Failed to scan access data");
							}
								});

						request.reply(status_codes::OK, utility::conversions::to_string_t("Ok."));

						m_resplock.unlock();
					}
					else
					{
						request.reply(status_codes::NotFound, utility::conversions::to_string_t("Not found."));
						return;
					}
					});

				m_listener->open().wait();
			}

			~oauth2_code_listener() { m_listener->close().wait(); }

			pplx::task<bool> listen_for_code() { return pplx::create_task(m_tce); }

		private:
			std::unique_ptr<http_listener> m_listener;
			pplx::task_completion_event<bool> m_tce;
			std::shared_ptr<oauth2_config>& m_config;
			std::mutex m_resplock;
		};
	};

	OAuth2Authorization::OAuth2Authorization()
	{
		impl = std::make_unique<OAuth2Authorization::_impl>();
	}

	OAuth2Authorization::OAuth2Authorization(const struct AccessData& acd)
	{
		impl.reset(new _impl(acd));
	}

	OAuth2Authorization::OAuth2Authorization(const String client_key, const String client_secret, const String redirect_uri, const String useragent)
	{
		impl = std::make_unique<_impl>(
			client_key,
			client_secret,
			redirect_uri,
			useragent
			);
	}

	OAuth2Authorization::OAuth2Authorization(OAuth2Authorization& other)
	{
		impl.reset(other.impl.get());
	}

	OAuth2Authorization::OAuth2Authorization(OAuth2Authorization&& other)
	{
		impl.reset(std::move(other.impl.get()));
	}


	void OAuth2Authorization::setUserCredentials(const String& Username, const String& Password)
	{
		impl->Username = std::move(Username);
		impl->Password = std::move(Password);
	}

	const String OAuth2Authorization::Username() { return impl->Username; }
	const String OAuth2Authorization::Password() { return impl->Password; }
	
	void OAuth2Authorization::setClientKey(const String& client_key) { impl->m_oauth2_config->set_client_key(std::move(client_key)); }

	const String OAuth2Authorization::ClientKey() { return impl->m_oauth2_config->client_key(); }

	const String OAuth2Authorization::ClientSecret() { return impl->m_oauth2_config->client_secret(); }
	void OAuth2Authorization::setClientSecret(const String& client_secret) { impl->m_oauth2_config->set_client_secret(std::move(client_secret)); }

	const String OAuth2Authorization::UserAgent() { return impl->m_oauth2_config->user_agent(); }
	void OAuth2Authorization::setUserAgent(const String& useragent) { impl->m_oauth2_config->set_user_agent(std::move(useragent)); }

	const String OAuth2Authorization::Scope() { return impl->m_oauth2_config->scope(); }
	void OAuth2Authorization::setScope(const String& scope) { impl->m_oauth2_config->set_scope(std::move(scope)); }

	bool OAuth2Authorization::RequestTokenFromPassword() { return impl->GetTokenFromPassword(); }
	bool OAuth2Authorization::RequestTokenFromCode() { return impl->GetTokenFromCode(); }

	class OAuth2Authorization::_impl : RateTracker
	{
	public:
		std::unique_ptr<Internal::oauth2_code_listener> m_listener;
		std::shared_ptr<oauth2_config> m_oauth2_config;
		String Username, Password;


		_impl() : RateTracker(60)
		{
			m_oauth2_config = std::make_shared<oauth2_config>(
				""_u,
				""_u,
				"https://www.reddit.com/api/v1/authorize"_u,
				"https://www.reddit.com/api/v1/access_token"_u,
				""_u
				);
			m_oauth2_config->set_bearer_auth(true);

		}

		_impl(const struct AccessData& acd) : RateTracker(60)
		{
			m_oauth2_config = std::make_shared<oauth2_config>(
				std::move(acd.client_key), 
				std::move(acd.client_secret), 
				"https://www.reddit.com/api/v1/authorize"_u, 
				"https://www.reddit.com/api/v1/access_token"_u, 
				std::move(acd.redirect_uri)
				);
			m_oauth2_config->set_user_agent(std::move(acd.useragent));
			m_oauth2_config->set_bearer_auth(true);
		}


		void GetTimeAndTrack()
		{
			SetMaxTime(
				std::chrono::seconds(m_oauth2_config->token().expires_in())
			);
			SetMaxTries(600);
			Track();
		}
		


		bool GetTokenFromPassword() {
			if (
				!m_oauth2_config->client_key().empty() &&
				!m_oauth2_config->client_secret().empty() &&
				!Username.empty() && !Password.empty() &&
				!m_oauth2_config->user_agent().empty()				
				) {
				try {
					m_oauth2_config->set_http_basic_auth(true);
					m_oauth2_config->token_from_password(this->Username, this->Password);
					if (m_oauth2_config->token().is_valid_access_token())
					{
						GetTimeAndTrack();
						return true;
					}
				}
				catch (std::exception& e) {
					scarlettNestedThrow(e.what());
				}
			}
			else {
				scarlettThrow("Invalid parameters: Username, Password, Client Key, Secret or User Agent are not present.");
			}
			return false;
		}

		void open_browser(utility::string_t auth_uri)
		{
#if defined(_WIN32) && !defined(__cplusplus_winrt)
			// NOTE: Windows desktop only.
			auto r = ShellExecuteW(NULL, L"open", auth_uri.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif defined(__APPLE__)
			// NOTE: OS X only.
			// NOTE: OS X only.
			string_t browser_cmd(utility::conversions::to_string_t("open \"") + auth_uri + utility::conversions::to_string_t("\""));
			(void)system(browser_cmd.c_str());
#else
			// NOTE: Linux/X11 only.
			utility::string_t browser_cmd(utility::conversions::to_string_t("xdg-open \"") + auth_uri + utility::conversions::to_string_t("\""));
			(void)system(browser_cmd.c_str());
#endif
		}

		void open_browser_auth()
		{
			auto auth_uri(m_oauth2_config->build_authorization_uri(true));
			open_browser(auth_uri);
		}

		pplx::task<bool> Authorize()
		{
			open_browser_auth();
			return m_listener->listen_for_code();
		}

		bool GetTokenFromCode() {
			if (!m_oauth2_config->client_key().empty() && 
				!m_oauth2_config->user_agent().empty()) {
				if(!m_listener)
					m_listener = std::make_unique<Internal::oauth2_code_listener>(m_oauth2_config->redirect_uri(), m_oauth2_config);

				if (Authorize().get())
				{
					m_oauth2_config->build_authorization_uri(true);
					if (m_oauth2_config->token().is_valid_access_token())
					{
						GetTimeAndTrack();
						return true;
					}
				}
				else
				{
					scarlettThrow("Authorization failed!");
				}
			}
			else {
				scarlettThrow("The client key or User Agent are not present!");
			}
			return false;
		}
	};
}