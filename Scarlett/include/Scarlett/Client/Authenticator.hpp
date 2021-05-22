#pragma once
#include <memory>
#include <mutex>
#include "cpprest/http_client.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "../Internal/Helpers.hpp"

using WString =  utility::string_t;

//
// A simple listener class to capture OAuth 2.0 HTTP redirect to localhost.
// The listener captures redirected URI and obtains the token.
// This type of listener can be implemented in the back-end to capture and store tokens.
//
class oauth2_code_listener
{
public:
    using URI = web::uri;
    using oauth2_config = web::http::oauth2::experimental::oauth2_config;
    using http_listener = web::http::experimental::listener::http_listener;
    using http_request = web::http::http_request;
    using oauth2_exception = web::http::oauth2::experimental::oauth2_exception;
    using status_codes = web::http::status_codes;


    oauth2_code_listener(URI listen_uri, oauth2_config& config)
        : m_listener(new http_listener(listen_uri)), m_config(config)
    {
        m_listener->support([this](http_request request) -> void {
            if (request.request_uri().path() == U("/"))
            {
                m_resplock.lock();

                m_config.token_from_redirected_uri(request.request_uri())
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
                    }
                        });

                request.reply(status_codes::OK, U("Ok."));

                m_resplock.unlock();
            }
            else
            {
                request.reply(status_codes::NotFound, U("Not found."));
            }
            });

        m_listener->open().wait();
    }

    ~oauth2_code_listener() { m_listener->close().wait(); }

    pplx::task<bool> listen_for_code() { return pplx::create_task(m_tce); }

private:
    std::unique_ptr<http_listener> m_listener;
    pplx::task_completion_event<bool> m_tce;
    oauth2_config& m_config;
    std::mutex m_resplock;
};
