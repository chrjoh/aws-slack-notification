#pragma once
#include <curl/curl.h>

#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "slack.hpp"

class Cloudwatch : public Slack {
   private:
    std::optional<std::string> accountName;
    virtual char *currentTime();

   public:
    Cloudwatch(std::string const &slackChannel, std::optional<std::string> accountName);
    void parse(nlohmann::json const &message);
    std::optional<std::string> message();
    virtual ~Cloudwatch();
};
