#pragma once

#include "Authenticator.hpp"
#include "IntervalTracker.hpp"
#include "cpprest/http_client.h"
#include "cpprest/http_listener.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <mutex>
#include "../Internal/Exceptions.hpp"


namespace Scarlett::Client
{
    using URI = web::uri;
    using oauth2_config = web::http::oauth2::experimental::oauth2_config;
    using http_listener = web::http::experimental::listener::http_listener;
    using http_request = web::http::http_request;
    using oauth2_exception = web::http::oauth2::experimental::oauth2_exception;
    using status_codes = web::http::status_codes;
    using http_client_config = web::http::client::http_client_config;

#if defined(_WIN32) && !defined(__cplusplus_winrt)
    // Extra includes for Windows desktop.
#include <windows.h>

#include <Shellapi.h>
#endif


//
// Utility method to open browser on Windows, OS X and Linux systems.
//
    static void open_browser(utility::string_t auth_uri)
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

    class Authorization;
    class PasswordGrant;


    template<typename T>
    class OAuth2Helper : public RateTracker
    {
    public:

        OAuth2Helper(const String client_key, const String secret, const String redirect_uri, const String useragent) : RateTracker(60)
        {
            init(client_key, secret, redirect_uri, useragent);
	        RateTracker::SetMaxTries(600);
        }

        OAuth2Helper(const String username, const String password, const String client_key, const String secret, const String redirect_uri, const String useragent) : RateTracker(60)
        {
            setUserCredentials(username, password);
            init(client_key, secret, redirect_uri, useragent);
	        RateTracker::SetMaxTries(600);
        }

        inline void setUserCredentials(const String& username, const String& password)
        {
            this->Username = username;
            this->Password = password;
        }
        inline String getUsername() { return Username; }
        inline String getPassword() { return Password; }

        inline void setClientKey(const String client_key) { m_oauth2_config->set_client_key(client_key); }
        inline String getClientKey() { return m_oauth2_config->client_key(); }
        
        inline void setClientSecret(const String client_secret) { m_oauth2_config->set_client_secret(client_secret); }
        inline String getClientSecret() { return m_oauth2_config->client_secret(); }

        inline void setUserAgent(const String useragent) { m_oauth2_config->set_user_agent(useragent); }
        inline const String getUserAgent() { return m_oauth2_config->user_agent(); }

        inline void setScope(const String scope) { m_oauth2_config->set_scope(scope); }
        inline const String getScope() { return m_oauth2_config->scope(); }

        inline void GetToken()
        {
            if (is_enabled())
            {
                if (!m_oauth2_config->token().is_valid_access_token())
                {
                    if constexpr (std::is_same_v<T, PasswordGrant>)
                    {
                        try {
                            m_oauth2_config->token_from_password(this->Username, this->Password);
                        }
                        catch (std::exception& e) {
                            scarlettNestedThrow(e.what());
                        }
                    }
                    else {
                        if (Authorize().get())
                        {
                            m_http_config.set_oauth2(*m_oauth2_config);
                            ucout << m_oauth2_config->token().access_token();
                        }
                        else
                        {
                            scarlettThrow("Authorization failed!");
                        }
                    }
                }
            }
            else
            {
                scarlettThrow("Invalid parameters: Username, Password, Client Key or Secret are not present.");
            }
        }

        inline pplx::task<bool> Authorize()
        {
            open_browser_auth();
            return m_listener->listen_for_code();
        }

        inline const http_client_config getSettings()
        {
            return m_http_config;
        }

    private:
        inline void open_browser_auth()
        {
            auto auth_uri(m_oauth2_config->build_authorization_uri(true));
            open_browser(auth_uri);
        }

        http_client_config m_http_config;
        std::shared_ptr<oauth2_config> m_oauth2_config;

        String Username, Password;

        inline void init(const String client_key, const String client_secret, const String redirect_uri, const String useragent)
        {
            if constexpr (std::is_same<T, PasswordGrant>::value)
            {
                m_oauth2_config = std::make_shared<oauth2_config>(
                    client_key,
                    client_secret,
                    ""_u,
                    "https://www.reddit.com/api/v1/access_token"_u,
                    redirect_uri
                    );
                m_oauth2_config->set_http_basic_auth(true);
            }
            else {
                m_oauth2_config = std::make_shared<oauth2_config>(client_key, client_secret, "https://www.reddit.com/api/v1/authorize"_u, "https://www.reddit.com/api/v1/access_token"_u, redirect_uri);
		        m_listener = std::make_unique<oauth2_code_listener>(m_oauth2_config->redirect_uri(), m_oauth2_config);
            }

            m_oauth2_config->set_bearer_auth(true);
            m_oauth2_config->set_user_agent(useragent);
        }


        inline bool is_enabled() const
        {
            if constexpr (std::is_same_v<T, Authorization>)
            {
                return !m_oauth2_config->client_key().empty(); 
            }
            else {
                return !m_oauth2_config->client_key().empty() && !m_oauth2_config->client_secret().empty() && !Username.empty() && !Password.empty();
            }

        }
        std::unique_ptr<oauth2_code_listener> m_listener;
    };
}
