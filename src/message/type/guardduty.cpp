#include "guardduty.hpp"

Guardduty::Guardduty(std::string const &slackChannel, std::optional<std::string> accountName) : Slack(slackChannel, accountName) {
}
void Guardduty::parse(nlohmann::json const &message) {
    using nljson = nlohmann::json;
    nljson localJson;
    std::string region = message["region"];
    std::string subject = "AWS GuardDuty Alert";
    std::string account = message["account"];

    localJson["text"] = "*" + subject + " in " + region + "*";
    localJson["channel"] = channel();

    if (message.contains("detail")) {
        nlohmann::json detail = message["detail"];
        std::string type = detail["type"];
        std::string title = detail["title"];
        std::string description = detail["description"];
        int severity = detail["severity"];
        std::string headerSubject = headerTitle(subject, region, account);
        localJson["attachments"] = nljson::array();
        localJson["attachments"].push_back(nljson::object({{"color", "#FF0000"}}));
        localJson["attachments"][0]["blocks"] = nljson::array();
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", headerSubject}}}}));
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Alert title*\n" + title}}}}));
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Severity*\n" + std::to_string(severity)}}}}));
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Type*\n" + type}}}}));
        localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Description*\n" + description}}}}));
    }
    json = localJson;
}
std::string Guardduty::headerTitle(std::string const &subject, std::string const &region, std::string const &account) {
    std::string name = accountName.has_value() ? accountName.value() : account;
    return subject + " in region " + region + "(" + name + ")";
}

std::optional<std::string> Guardduty::message() {
    return json.has_value() ? json.value().dump() : std::optional<std::string>{};
}

Guardduty::~Guardduty() {
}
