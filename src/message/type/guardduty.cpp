#include "guardduty.hpp"

Guardduty::Guardduty(std::string const &slackChannel) : Slack(slackChannel) {
}
void Guardduty::parse(nlohmann::json const &message) {
    using nljson = nlohmann::json;
    nljson localJson;
    std::string region = message["region"];
    std::string subject = "AWS GuardDuty Alert";

    localJson["text"] = "*" + subject + " in " + region + "*";
    localJson["channel"] = channel();

    if (message.contains("detail")) {
        nlohmann::json detail = message["detail"];
        std::string type = detail["type"];
        std::string title = detail["title"];
        std::string description = detail["description"];
        int severity = detail["severity"];
        localJson["attachments"] = nljson::array();
        localJson["attachments"].push_back(nljson::object({{"color", "#FF0000"}}));
        localJson["attachments"][0]["blocks"] = nljson::array();
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", subject}}}}));
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Alert title*\n" + title}}}}));
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Severity*\n" + std::to_string(severity)}}}}));
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Type*\n" + type}}}}));
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Description*\n" + description}}}}));
    }
    json = localJson;
}

std::optional<std::string> Guardduty::message() {
    return json.has_value() ? json.value().dump() : std::optional<std::string>{};
}

Guardduty::~Guardduty() {
}
