#pragma once

#include "Authenticator.hpp"
#include "cpprest/http_client.h"
#include "cpprest/http_listener.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <mutex>


namespace Scarlett
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
        string_t browser_cmd(U("open \"") + auth_uri + U("\""));
        (void)system(browser_cmd.c_str());
#else
        // NOTE: Linux/X11 only.
        string_t browser_cmd(U("xdg-open \"") + auth_uri + U("\""));
        (void)system(browser_cmd.c_str());
#endif
    }

    class Code;
    class _Password;


    template<typename T>
    class OAuth2Helper
    {
    public:
        OAuth2Helper(const WideString client_key, const WideString secret, const WideString redirect_uri, const WideString useragent)
        {
            init(client_key, secret, redirect_uri, useragent);
        }

        OAuth2Helper(const WideString username, const WideString password, const WideString client_key, const WideString secret, const WideString redirect_uri, const WideString useragent)
        {
            setUserCredentials(username, password);
            init(client_key, secret, redirect_uri, useragent);
        }

        inline void init(const WideString client_key, const WideString secret, const WideString redirect_uri, const WideString useragent)
        {
            if constexpr (std::is_same<T, _Password>::value)
            {
                auto tokenEndpoint = "/api/v1/access_token" + GenerateParamData(StringMap{
                    {"grant_type", "password"},
                    {"username", u8(Username)},
                    {"password", u8(Password) }});

                m_oauth2_config = std::make_unique<oauth2_config>(
                    client_key,
                    secret,
                    ""_u,
                    utility::conversions::to_string_t(tokenEndpoint),
                    redirect_uri
                    );

                m_oauth2_config->set_bearer_auth(true);
            }
            else {
                m_listener = std::make_unique<oauth2_code_listener>(redirect_uri, *m_oauth2_config);
                m_oauth2_config = std::make_unique<oauth2_config>(client_key, client_secret, auth_endpoint, token_endpoint, redirect_uri);
            }
        }

        inline void GetToken()
        {
            if (is_enabled())
            {
                if (!m_oauth2_config->token().is_valid_access_token())
                {
                    if constexpr (std::is_same_v<T, _Password>)
                    {
                        m_http_config.set_oauth2(*m_oauth2_config);
                        //m_oauth2_config->token_from_refresh();

                        http_request req(HttpMethod::POST);
                        req.set_request_uri("https://www.reddit.com/api/v1/access_token"_u);

                        HttpClient cl("https://www.reddit.com"_u, m_http_config);
                        cl.request(req);

                    }
                    else {
                        if (Authorize().get())
                        {
                            m_http_config.set_oauth2(*m_oauth2_config);
                            if (confidential)
                            {
                                // TODO: Implement AOA/confidential specific token retrieval
                            }
                            else {
                                ucout << m_oauth2_config->token().access_token();
                            }
                        }
                        else
                        {
                            ucout << "Authorization failed for " << m_name.c_str() << "." << std::endl;
                        }
                    }
                }
            }
            else
            {
                ucout << "Skipped " << m_name.c_str()
                    << " session sample because app key or secret is empty. Please see instructions." << std::endl;
            }
        }

        inline void setUserCredentials(const WideString& username, const WideString& password)
        {
            this->Username = username;
            this->Password = password;
        }

        inline void setImplicitGrant(bool option)
        {
            this->generate_state = true;
            m_oauth2_config->set_implicit_grant(true);
        }

        inline void setConfidential(bool option)
        {
            this->confidential = option;
        }

    protected:
        inline pplx::task<bool> Authorize()
        {
            open_browser_auth();
            return m_listener->listen_for_code();
        }

        inline void open_browser_auth()
        {
            auto auth_uri(m_oauth2_config->build_authorization_uri(generate_state));
            open_browser(auth_uri);
        }

        http_client_config m_http_config;
        std::unique_ptr<oauth2_config> m_oauth2_config;

    private:
        WideString Username, Password;
        bool generate_state, implicitgrant, confidential;

        inline bool is_enabled() const
        {
            {
                if(implicitgrant)
                {
                    return !m_oauth2_config->client_key().empty();
                }
                else {
                    return !m_oauth2_config->client_key().empty() && !m_oauth2_config->client_secret().empty();
                }
            }

        }
        utility::string_t m_name;
        std::unique_ptr<oauth2_code_listener> m_listener;
    };
}