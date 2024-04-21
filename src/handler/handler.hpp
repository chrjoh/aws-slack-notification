#pragma once

#include <algorithm>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>
#include <string>

#include "awshealth.hpp"
#include "client.hpp"
#include "cloudwatch.hpp"
#include "environment.hpp"
#include "guardduty.hpp"
#include "inspector.hpp"
#include "slack.hpp"
class Handler {
   private:
    std::string const slackUrl = "https://slack.com/api/chat.postMessage";
    std::string const parameterUrl = "http://localhost:2773/systemsmanager/parameters/get";
    std::shared_ptr<HttpClient> httpclient;
    bool haveSnSMessage(nlohmann::json &data) const;
    Environment env;

   public:
    Handler(std::shared_ptr<HttpClient> httpclient, Environment &env);
    void process(std::string const &payload);
    std::unique_ptr<Slack> parseAndCreateSlackMessage(std::string const &payload, std::string const &slackChannel);
    virtual ~Handler();
};
