#pragma once

#include "cpprest/http_client.h"
#include "cpprest/http_listener.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <mutex>
#include "../Internal/Helpers.hpp"
#include "../Internal/Exceptions.hpp"

#if defined(_WIN32) && !defined(__cplusplus_winrt)
// Extra includes for Windows desktop.
#include <windows.h>

#include <Shellapi.h>
#endif


namespace Scarlett::Client
{
    struct AccessData
    {
        Scarlett::String client_key, client_secret, useragent, redirect_uri;
        Scarlett::String username, password;
    };


    class OAuth2Authorization
    {
    private:
        class _impl;
        std::unique_ptr<_impl> impl;
    public:
        using oauth2_config = web::http::oauth2::experimental::oauth2_config;
        using http_client_config = web::http::client::http_client_config;

        OAuth2Authorization();
        OAuth2Authorization(const String client_key, const String client_secret, const String redirect_uri, const String useragent);
        OAuth2Authorization(const struct AccessData& ad);
        OAuth2Authorization(OAuth2Authorization& other);
        OAuth2Authorization(OAuth2Authorization&& other);


        void setUserCredentials(const String& username, const String& password);

        const String Username();
        const String Password();

        void setClientKey(const String& client_key);
        const String ClientKey();
        
        void setClientSecret(const String& client_secret);
        const String ClientSecret();

        void setUserAgent(const String& useragent);
        const String UserAgent();

        void setScope(const String& scope);
        const String Scope();

        bool RequestTokenFromPassword();
        bool RequestTokenFromCode();

    };
}
