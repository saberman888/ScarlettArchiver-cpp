#include "BasicRequest.hpp"
#include <iostream>

namespace BR
{
    static size_t writedat(char* buffer, size_t size, size_t nmemb, std::vector<std::byte>& dest)
    {
        dest.push_back(static_cast<std::byte>(*buffer));
        return size * nmemb;
    }


    template<RequestType T>
    BasicRequest<T>& operator<<(BasicRequest<T>& br,  std::tuple<std::string, std::string> parameter)
    {
        std::string temp = std::get<0>(parameter) + "=" + std::get<1>(parameter);
        curl_url_set(br.URLHandle, CURLUPART_QUERY, temp.c_str(), CURLU_APPENDQUERY | CURLU_URLENCODE);
        return br;
    }


    template<RequestType T>
    BasicRequest<T>::BasicRequest(const UrlScheme& Urlpieces) : headers(nullptr)
    {
        std::call_once(ginitFlag, []() { if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) { throw std::runtime_error("curl_global_init failed to initialize!"); atexit(curl_global_cleanup); }});
        
        URLHandle = curl_url();
        _Response = std::make_unique<Response>();
        
        curl_url_set(URLHandle, CURLUPART_SCHEME, std::get<0>(Urlpieces).c_str(), 0);
        curl_url_set(URLHandle, CURLUPART_HOST, std::get<1>(Urlpieces).c_str(), 0);
        curl_url_set(URLHandle, CURLUPART_PORT, std::get<2>(Urlpieces).c_str(), 0);
        curl_url_set(URLHandle, CURLUPART_PATH, std::get<3>(Urlpieces).c_str(), 0);
        Setup();
        SetOpt<CURLOPT_TIMEOUT>(30L);
    }


    template<RequestType T>
    void BasicRequest<T>::Setup()
    {}

    template<>
    void BasicRequest<RequestType::POST>::Setup()
    {
        //Handle = curl_easy_init();
        if(!Handle)
        {
            throw std::runtime_error("Error, failed to initialize curl_easy handle");
        } else {
            SetOpt<CURLOPT_POST>(1L);
        }
    }

    template<>
    void BasicRequest<RequestType::GET>::Setup()
    {
        Handle = curl_easy_init();
        if(!Handle)
        {
            throw std::runtime_error("Error, failed to initialize curl_easy handle");
        } else {
            SetOpt<CURLOPT_HTTPGET>(1L);
        }
    }

    template<RequestType T>
    void BasicRequest<T>::Cleanup()
    {
        if(headers)
            curl_slist_free_all(headers);

        if (Handle)
            curl_easy_cleanup(Handle);

        if (URLHandle)
            curl_url_cleanup(URLHandle);

        // re-assign to nullptr
        headers = nullptr;
        Handle = nullptr;
        URLHandle = nullptr;
    }

    template<RequestType T>
    void BasicRequest<T>::addHeader(const std::string& name, const std::string& value)
    {
        auto fullheader = name + ": " + value;
        headers = curl_slist_append(headers, fullheader.c_str());
    }

    template<RequestType T>
    void BasicRequest<T>::WriteToState()
    {
        SetOpt(CURLOPT_WRITEFUNCTION, &writedat);
        SetOpt(CURLOPT_WRITEDATA, Response->buffer);
    }

    template<RequestType T>
    void BasicRequest<T>::setPostfields(std::string params)
    {
        SetOpt(CURLOPT_COPYPOSTFIELDS, params.c_str());
    }

    template<RequestType T>
    void BasicRequest<T>::setUserCredentials(const std::string& Username, const std::string& Password)
    {
        //SetOpt(CURLOPT_USERPWD, Username + ":" + Password);
        curl_url_set(URLHandle, CURLUPART_USER, Username.c_str(), 0);
        curl_url_set(URLHandle, CURLUPART_PASSWORD, Password.c_str(), 0);
    }

    template<RequestType T>
    void BasicRequest<T>::SetUserAgent(std::string useragent)
    {
        SetOpt(CURLOPT_USERAGENT, useragent.c_str());
    }


    template<RequestType T>
    CURLcode BasicRequest<T>::Perform()
    {
        if(headers)
            SetOpt(CURLOPT_HTTPHEADER, headers);
        char* extractedURL = nullptr;
        curl_url_get(URLHandle, CURLUPART_URL, &extractedURL, CURLU_URLDECODE);
        curl_easy_setopt(Handle, CURLOPT_URL, &extractedURL);
        auto cURLResponse = curl_easy_perform(Handle);
        return cURLResponse;
    }

    template<RequestType T>
    std::shared_ptr<Response> BasicRequest<T>::SendRequest()
    {
        auto errcode = Perform();
        
        if (errcode != CURLE_OK) {
            _Response->message = curl_easy_strerror(errcode);
        }

        if (errcode != CURLE_OPERATION_TIMEDOUT)
        {
            GetInfo(CURLINFO_RESPONSE_CODE, &(_Response->HttpState));
            char* tempContentType = nullptr;
            GetInfo(CURLINFO_CONTENT_TYPE, &tempContentType);

            /*if (tempContentType)
                _Response->ContentType = tempContentType;*/
        }
        
        return _Response;
    }

    template<RequestType T>
    time_t BasicRequest<T>::GetUTCTime(const std::string data)
    {
        auto time = curl_getdate(data.c_str(), nullptr);
        if(!time)
        {
            throw std::runtime_error("Error: Got NULL from curl_getdate instead of time_t. Possible invalid date string format");
        } else {
            return time;
        }
    }

    template<RequestType T>
    time_t BasicRequest<T>::GetCurrentUTCTime()
    {
        time_t data;
        time(&data);
        return data;
    }
    
    template<RequestType T>
    std::string BasicRequest<T>::UTCToString(time_t time, std::string format)
    {
    #if defined(_MSC_VER)
        struct tm timeinfo;
    #else
        struct tm* timeinfo;
    #endif

        char buffer[80];
    #if defined(_MSC_VER)
        localtime_s(&timeinfo, &time);
        strftime(buffer, 80, format.c_str(), &timeinfo);
    #else
        timeinfo = std::gmtime(&time);
        std::strftime(buffer, 80, format.c_str(), timeinfo);
    #endif

        return std::string(buffer);
    }
}


