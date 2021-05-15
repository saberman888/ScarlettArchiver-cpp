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

    class Authorization;
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

        inline void setUserCredentials(const WideString& username, const WideString& password)
        {
            this->Username = username;
            this->Password = password;
        }
        inline WideString getUsername() { return Username; }
        inline WideString getPassword() { return Password; }

        inline void setClientKey(const WideString client_key) { m_oauth2_config->set_client_key(client_key); }
        inline WideString getClientKey() { return m_oauth2_config->client_key(); }
        
        inline void setClientSecret(const WideString client_secret) { m_oauth2_config->set_client_secret(client_secret); }
        inline WideString getClientSecret() { m_oauth2_config->client_secret(); }

        inline void setImplicitGrant(bool option)
        {
            this->generate_state = true;
            m_oauth2_config->set_implicit_grant(true);
        }
        inline void ImplicitGrant() { return m_oauth2_config->implicit_grant(); }

        inline void setConfidential(bool option)
        {
            this->confidential = option;
        }
        inline bool Confidential() {
            return this->confidential;
        }

        inline void enablePermanence(bool option = true)
        {
            permanentToken = true;
        }
        inline bool Permanent() {
            return permanentToken;
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
                        m_http_config.set_credentials(
                            web::credentials(
                                m_oauth2_config->client_key(),
                                m_oauth2_config->client_secret()
                            )
                        );

                        http_request req(HttpMethod::POST);

                        web::uri_builder ub;
                        ub.append("https://www.reddit.com/api/v1/access_token"_u);
                        ub.append_query("grant_type=password"_u);
                        ub.append_query("username="_u + Username);
                        ub.append_query("password="_u + Password);

                        req.set_request_uri(ub.to_uri());

                        HttpClient cl("https://www.reddit.com"_u, m_http_config);
                        auto response = cl.request(req).get();

                        if (response.status_code() != 200)
                        {

                        }
                        else {
                            readToken(response.extract_json().get());
                        }


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
        bool generate_state, implicitgrant, confidential, permanentToken;

        inline void init(const WideString client_key, const WideString client_secret, const WideString redirect_uri, const WideString useragent)
        {
            if constexpr (std::is_same<T, _Password>::value)
            {
                m_oauth2_config = std::make_unique<oauth2_config>(
                    client_key,
                    client_secret,
                    ""_u,
                    "https://www.reddit.com/api/v1/access_token"_u,
                    redirect_uri
                    );
            }
            else {
                authorizationEndpoint = URI("https://www.reddit.com/api/v1/authorize"_u);
                m_listener = std::make_unique<oauth2_code_listener>(redirect_uri, *m_oauth2_config);
                m_oauth2_config = std::make_unique<oauth2_config>(client_key, client_secret, "https://www.reddit.com/api/v1/authorize"_u, "https://www.reddit.com/api/v1/access_token"_u, redirect_uri);
            }

            m_oauth2_config->set_bearer_auth(true);
            m_oauth2_config->set_user_agent(useragent);
        }

        inline void readToken(const JSON::value data)
        {
            if (data.has_field("error"_u))
            {
                // TODO: Implement invalid grant handling
            }
            else {
                web::http::oauth2::experimental::oauth2_token t;

                t.set_access_token(data.at("access_token"_u).as_string());
                t.set_expires_in(data.at("expires_in"_u).as_integer());
                t.set_token_type(data.at("token_type"_u).as_string());
                t.set_scope(data.at("scope"_u).as_string());
                m_oauth2_config->set_token(t);
            }
        }

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

    using SimpleClient = OAuth2Helper<_Password>;
    using AuthClient = OAuth2Helper<Authorization>;
}