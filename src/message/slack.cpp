#include "slack.hpp"

Slack::Slack(std::string const &slackChannel, std::optional<std::string> accountName) : slackChannel(slackChannel), accountName(accountName) {
}

Slack::Slack(std::string const &slackChannel, bool const &useErrorMessage) : slackChannel(slackChannel), useErrorMessage(useErrorMessage) {
}
std::optional<std::string> Slack::message() {
    if (useErrorMessage) {
        return CreateErrorMessage();
    }
    return std::optional<std::string>();
}

std::string Slack::channel() {
    return slackChannel;
}

std::string Slack::headerTitle(std::string const &subject, std::string const &region, std::string const &account) const {
    std::string name = accountName.has_value() ? accountName.value() : account;
    return subject + " in region " + region + "(" + name + ")";
}

void Slack::parse(nlohmann::json const &message) {}

nlohmann::json Slack::getJson() {
    if (json.has_value()) {
        return json.value();
    } else {
        return nlohmann::json();
    }
}

std::string Slack::CreateErrorMessage() {
    using nljson = nlohmann::json;
    nljson localJson;

    localJson["text"] = "* Error parsing *";
    localJson["channel"] = channel();

    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Description*\n Failed to parse incoming message check the logs for the lambda function"}}}}));
    return localJson.dump();
}

Slack::~Slack() {
}