#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "slack.hpp"

class Inspector : public Slack {
   private:
    double serverityThreshold;
    bool onlyLatest;
    enum class FindingType { AWS_LAMBDA_FUNCTION,
                             AWS_ECR_CONTAINER_IMAGE,
                             INITIAL_SCAN_COMPLETE,
                             UNKNOWN };

    class Message {
       public:
        Message(nlohmann::json const &message);
        std::string detailType;
        std::string region;
        std::string account;
        std::string resource;
        std::string description;
        std::string exploitAvailable;
        std::string fixAvailable;
        float score{0};
        std::string inspectorScore = "--";
        std::string severity;
        std::string vulnerability;
        std::string subject;
    };

    FindingType typeOffinding(nlohmann::json const &message);
    bool hasFindingResources(nlohmann::json const &message);
    void parseLambdaMessage(nlohmann::json const &message);
    void parseECRMessage(nlohmann::json const &message);
    void parseInitialScanMessage(nlohmann::json const &message);
    void createECRorLambdaSlackMessage(nlohmann::json &localJson, Inspector::Message const &extracted);

   public:
    Inspector(bool const &onlyLatest, double const &serverityThreshold, std::string const &slackChannel, std::optional<std::string> accountName);
    void parse(nlohmann::json const &message);
    std::optional<std::string> message();
    ~Inspector();
};
