#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "slack.hpp"

class Guardduty : public Slack {
   private:
   public:
    Guardduty(std::string const &slackChannel, std::optional<std::string> accountName);
    void parse(nlohmann::json const &message);
    std::optional<std::string> message();
    ~Guardduty();
};
