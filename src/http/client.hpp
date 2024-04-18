#pragma once
#include <curl/curl.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

typedef std::map<std::string, std::string> Params;
class HttpClient {
   private:
   public:
    HttpClient();
    std::string paramsString(Params const &params);
    virtual int doPostToSlack(std::string const &url, std::string const &token, std::string const &msg);
    virtual nlohmann::json doGetSecretParameter(std::string const &url, std::string const &token, Params const &params);
    virtual ~HttpClient();
};
