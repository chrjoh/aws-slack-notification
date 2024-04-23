#include "inspector.hpp"

#include <iomanip>
#include <sstream>

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
    std::string account = message["account"];

    std::string resource = message["resources"][0];
    auto detail = message["detail"];
    std::string description = detail["description"];
    std::string exploitAvailable = detail["exploitAvailable"];
    std::string fixAvailable = detail["fixAvailable"];
    float score = detail["inspectorScore"];
    std::stringstream s;
    s << std::setprecision(2) << score;
    std::string inspectorScore = s.str();
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
            std::string subject = detailType + " for ECR image";
            std::string headerSubject = headerTitle(subject, region, account);
            localJson["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", headerSubject}}}}));
            break;
        }
        case FindingType::AWS_LAMBDA_FUNCTION: {
            std::string subject = detailType + " for lambda function";
            std::string headerSubject = headerTitle(subject, region, account);
            localJson["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", headerSubject}}}}));
            break;
        }
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
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Inspector score*\n" + inspectorScore}}));
    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"fields", fields}}));

    if (score >= serverityThreshold) {
        if (onlyLatest && !haveLatestTag) {
            json = {};
        } else {
            json = localJson;
        }
    }
}

Inspector::~Inspector() {
}
