#pragma once

#include "Authenticator.hpp"
#include "cpprest/http_client.h"
#include "cpprest/http_listener.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <mutex>

using WString = utility::string_t;
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

class ImplicitGrant;
class Code;
class AOA; // Application Only OAuth
class Password;


template<typename T>
class OAuth2Helper
{
public:


    OAuth2Helper(WString Name,
        WString client_key,
        WString client_secret,
        WString auth_endpoint,
        WString token_endpoint,
        WString redirect_uri);
    
    OAuth2Helper(WString Name,
        WString client_key,
        WString auth_endpoint,
        WString token_endpoint,
        WString redirect_uri);

    constexpr bool init(
                const WString Name,
                const WString client_key, 
                const WString client_secret, 
                const WString auth_endpoint, 
                const WString token_endpoint, 
                const WString redirect_uri);

    void run();

protected:
    pplx::task<bool> Authorize();
    void open_browser_auth();

    http_client_config m_http_config;
    std::unique_ptr<oauth2_config> m_oauth2_config;

private:
    bool generate_state;
    constexpr bool is_enabled() const;
    utility::string_t m_name;
    std::unique_ptr<oauth2_code_listener> m_listener;
};


template<typename T>
OAuth2Helper<T>::OAuth2Helper(const WString Name, const WString client_key, const WString client_secret, const WString auth_endpoint, const WString token_endpoint, const WString redirect_uri)
{
    if (!init(Name, client_key, client_secret, auth_endpoint, token_endpoint, redirect_uri))
    {
        // TODO: throw exception
    }
}

template<typename T>
OAuth2Helper<T>::OAuth2Helper(const WString Name, const WString client_key, const WString auth_endpoint, const WString token_endpoint, const WString redirect_uri)
{
    if (!init(Name, client_key, WString(), auth_endpoint, token_endpoint, redirect_uri))
    {
        // TODO: Throw exeception
    }
}

template<typename T>
constexpr bool OAuth2Helper<T>::init(const WString Name, const WString client_key, const WString client_secret, const WString auth_endpoint, const WString token_endpoint, const WString redirect_uri)
{
    m_name = Name;
    m_oauth2_config = std::make_unique<oauth2_config>(client_key, client_secret, auth_endpoint, token_endpoint, redirect_uri);
    m_listener = std::make_unique<oauth2_code_listener>(redirect_uri, *m_oauth2_config);
    if constexpr (std::is_same<T, ImplicitGrant>::value)
    {
        m_oauth2_config->set_implicit_grant(true);
        generate_state = true;
    }
    else if constexpr (std::is_same<T, Code>::value) {
        generate_state = false;
    }
    else {
        generate_state = false;
    }
    return true;
}

template<typename T>
constexpr bool OAuth2Helper<T>::is_enabled() const
{
    if constexpr (std::is_same<T, ImplicitGrant>::value)
    {
        return !m_oauth2_config->client_key().empty();
    }
    else {
        return !m_oauth2_config->client_key()->empty() && !m_oauth2_config->client_secret()->empty();
    }
}

template<typename T>
void OAuth2Helper<T>::open_browser_auth()
{
    auto auth_uri(m_oauth2_config->build_authorization_uri(generate_state));
    open_browser(auth_uri);
}

template<typename T>
pplx::task<bool> OAuth2Helper<T>:: Authorize()
{
    open_browser_auth();
    return m_listener->listen_for_code();
}

template<typename T>
void OAuth2Helper<T>::run()
{
    if (is_enabled())
    {
        if (!m_oauth2_config->token().is_valid_access_token())
        {
            if (Authorize().get())
            {
                m_http_config.set_oauth2(*m_oauth2_config);
                if constexpr (std::is_same<T, AOA>::value)
                {
                    // TODO: Implement AOA specific token retrieval
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
    else
    {
        ucout << "Skipped " << m_name.c_str()
            << " session sample because app key or secret is empty. Please see instructions." << std::endl;
    }
}