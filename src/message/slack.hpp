#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

class Slack {
   private:
    /* data */
   protected:
    std::optional<nlohmann::json> json;
    std::string slackChannel;
    std::optional<std::string> accountName;
    bool useErrorMessage = false;
    std::string CreateErrorMessage();

   public:
    Slack(std::string const &slackChannel, std::optional<std::string> accountName);
    Slack(std::string const &slackChannel, bool const &useErrorMessage);
    virtual void parse(nlohmann::json const &message);
    virtual std::optional<std::string> message();
    std::string channel();
    nlohmann::json getJson();
    virtual ~Slack();
};
