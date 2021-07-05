#pragma once

#include "../Internal/Types.hpp"

namespace Scarlett::Client
{
    struct AccessData
    {
        Scarlett::String client_key, client_secret, useragent, redirect_uri;
        Scarlett::String username, password;
    };
}