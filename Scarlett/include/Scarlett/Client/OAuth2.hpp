#pragma once

#include "cpprest/oauth2.h"
#include "../Internal/Types.hpp"
#include "AccessData.hpp"
#include <memory>


#if defined(_WIN32) && !defined(__cplusplus_winrt)
// Extra includes for Windows desktop.
#include <windows.h>

#include <Shellapi.h>
#endif


namespace Scarlett::Client
{
    class SCDLL OAuth2Authorization
    {
    private:
        class _impl;
        std::shared_ptr<_impl> impl;
    public:
        using oauth2_config = web::http::oauth2::experimental::oauth2_config;

        OAuth2Authorization();
        OAuth2Authorization(const String client_key, const String client_secret, const String redirect_uri, const String useragent);
        OAuth2Authorization(const struct AccessData& ad);

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
