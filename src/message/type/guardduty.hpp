#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "slack.hpp"

class Guardduty : public Slack {
   private:
    std::string headerTitle(std::string const &subject, std::string const &region, std::string const &account);

   public:
    Guardduty(std::string const &slackChannel, std::optional<std::string> accountName);
    void parse(nlohmann::json const &message);
    std::optional<std::string> message();
    ~Guardduty();
};
