
#pragma once

#include "curl/curl.h"
#include <string>
#include <iosfwd>
#include <cstdlib>
#include <cassert>
#include <map>
#include <exception>
#include <ctime>
#include <mutex>
#include <memory>
#include <utility>
#include <vector>

namespace BR
{
    
    template<typename T>
    struct CURLDeleter
    {
        void operator()(T* handle);
    };
    
    template<typename T>
    void CURLDeleter<T>::operator()(T* handle){}
    
    template<>
    void CURLDeleter<CURL>::operator()(CURL* handle){ curl_easy_cleanup(handle);}
    
    template<>
    void CURLDeleter<CURLU>::operator()(CURLU* handle){ curl_url_cleanup(handle);}
    
    template<>
    void CURLDeleter<struct curl_slist>::operator()(struct curl_slist* handle){ curl_slist_free_all(handle);}
    
    
    
    
    typedef struct ___response
    {
        long HttpState;
        std::string message;
        std::vector<std::shared_ptr<std::byte>> buffer;
    }Response;

    enum class RequestType
    {
        GET,
        POST,
        PUT,
        PATCH
    };

    static std::once_flag ginitFlag;
    using UrlScheme = std::tuple<std::string, std::string, int, std::string>;

    /*
        Pretty basic cURL wrapper made to do one request at a time, which is ideal for this project
    */

    template<RequestType T>
    class BasicRequest
    {
    public:
        BasicRequest(const UrlScheme& Urlpieces);

        /*
            Adds headers to the cURL handle. It's equivalent to using curl_slist_append.
        */
        void addHeader(const std::string& name, const std::string& value);

        /*
            Wrapper for curl_easy_setopt. Throws std::runtime_error if anything goes wrong
        */
        template<CURLoption opt, typename Y>
        void SetOpt(Y data)
        {
            CURLcode result = curl_easy_setopt(Handle, opt, data);
            if (result != CURLE_OK)
            {
                throw std::runtime_error(curl_easy_strerror(result));
            }
        }
        /*
            Wrapper for curl_easy_getinfo. Throws std::runtime_error if anything goes wrong
        */
        template<CURLINFO opt, typename Y>
        void GetInfo(Y* data)
        {
            auto result = curl_easy_getinfo(Handle, opt, Y);
            if (result != CURLE_OK)
            {
                throw std::runtime_error(curl_easy_strerror(result));
            }
        }
        void setPostfields(std::string params);
        void setUserCredentials(const std::string& Username, const std::string& Password);
        void SetUserAgent(std::string useragent);


        std::shared_ptr<Response> SendRequest();
        void Cleanup();

        static time_t GetUTCTime(const std::string data);
        static time_t GetCurrentUTCTime();
        static std::string UTCToString(time_t time, std::string format);
        
        template<RequestType TT>
        friend BasicRequest<TT>& operator<<(BasicRequest<TT>& br,  std::tuple<std::string, std::string> parameter);

    private:
        /*
            Sets up the operation by initializing the cURL, initializes other internal variables like Response and 
            assigns it to be written to. You can also set it up as a POST request by passing true into the last parameter 
            and make cURL output more verbosely by passing NDEBUG=Off in CMake.

            Throws std::runtime_error if CURL* Handle fails to initialize.
        */
        void Setup();
        
        CURLcode Perform();
        
        CURL* Handle;
        CURLU* URLHandle;
        struct curl_slist* headers;
        std::shared_ptr<Response> _Response;
        
        void WriteToState();
    };


    template<RequestType T>
    BasicRequest<T>& operator<<(BasicRequest<T>& br,  std::tuple<std::string, std::string> parameter);
}
