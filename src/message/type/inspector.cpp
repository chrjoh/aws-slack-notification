#include "inspector.hpp"

Inspector::Inspector(bool const &onlyLatest, double const &serverityThreshold, std::string const &slackChannel, std::optional<std::string> accountName)
    : onlyLatest(onlyLatest), serverityThreshold(serverityThreshold), Slack(slackChannel, accountName) {
}

void Inspector::parse(nlohmann::json const &message) {
    auto resourceType = typeOffinding(message);
    auto somdata = message["detail"]["resources"][0];
    parseMessage(message, resourceType);
}

std::optional<std::string> Inspector::message() {
    return json.has_value() ? json.value().dump() : std::optional<std::string>{};
}

Inspector::FindingType Inspector::typeOffinding(nlohmann::json const &message) {
    if (hasFindingResources(message)) {
        if (message["detail"]["resources"][0]["type"] == "AWS_LAMBDA_FUNCTION") {
            return FindingType::AWS_LAMBDA_FUNCTION;
        } else if (message["detail"]["resources"][0]["type"] == "AWS_ECR_CONTAINER_IMAGE") {
            return FindingType::AWS_ECR_CONTAINER_IMAGE;
        }
    }
    return FindingType::UNKNOWN;
}

bool Inspector::hasFindingResources(const nlohmann::json &message) {
    return message.contains("detail") && message["detail"].contains("resources") && message["detail"]["resources"].size() > 0;
}

void Inspector::parseMessage(nlohmann::json const &message, FindingType const &findingType) {
    using nljson = nlohmann::json;
    nljson localJson;
    std::string detailType = message["detail-type"];
    std::string region = message["region"];

    std::string resource = message["resources"][0];
    auto detail = message["detail"];
    std::string description = detail["description"];
    std::string exploitAvailable = detail["exploitAvailable"];
    std::string fixAvailable = detail["fixAvailable"];
    float inspectorScore = detail["inspectorScore"];
    std::string severity = detail["severity"];
    std::string vulnerability = detail["title"];

    localJson["text"] = "*" + detailType + " in " + region + "*";
    localJson["channel"] = channel();
    localJson["blocks"] = nljson::array();
    bool haveLatestTag = true;
    switch (findingType) {
        case FindingType::AWS_ECR_CONTAINER_IMAGE: {
            auto tags = detail["resources"][0]["details"]["awsEcrContainerImage"]["imageTags"].get<std::vector<std::string>>();
            if ((std::find(begin(tags), end(tags), "latest") == std::end(tags))) {
                haveLatestTag = false;
            }
            localJson["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", detailType + " for ECR image in " + region}}}}));
            break;
        }
        case FindingType::AWS_LAMBDA_FUNCTION:
            localJson["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", detailType + " for lambda function in " + region}}}}));
            break;
        default:
            break;
    }

    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Description*\n" + description}}}}));
    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Vulnerability*\n" + vulnerability}}}}));
    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Resource*\n`" + resource + "`"}}}}));

    auto fields = nljson::array();
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Exploit available*\n" + exploitAvailable}}));
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Fix Available*\n" + fixAvailable}}));
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Severity*\n" + severity}}));
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Inspector score*\n" + std::to_string(inspectorScore)}}));
    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"fields", fields}}));

    if (inspectorScore >= serverityThreshold) {
        if (onlyLatest && !haveLatestTag) {
            json = {};
        } else {
            json = localJson;
        }
    }
}

Inspector::~Inspector() {
}
