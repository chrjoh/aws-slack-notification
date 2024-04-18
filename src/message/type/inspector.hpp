#pragma once
#include <algorithm>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

#include "slack.hpp"

class Inspector : public Slack {
   private:
    double serverityThreshold;
    bool onlyLatest;
    enum class FindingType { AWS_LAMBDA_FUNCTION,
                             AWS_ECR_CONTAINER_IMAGE,
                             UNKNOWN };
    FindingType typeOffinding(nlohmann::json const &message);
    bool hasFindingResources(nlohmann::json const &message);
    void parseMessage(nlohmann::json const &message, FindingType const &resourceType);

   public:
    Inspector(bool const &onlyLatest, double const &serverityThreshold, std::string const &slackChannel);
    void parse(nlohmann::json const &message);
    std::optional<std::string> message();
    ~Inspector();
};
