#include "awshealth.hpp"

Awshealth::Awshealth(std::string const &slackChannel, std::optional<std::string> accountName, std::vector<std::string> types) : Slack(slackChannel, accountName), eventTypes(types) {
}

void Awshealth::parse(nlohmann::json const &message) {
    using nljson = nlohmann::json;
    nljson localJson;
    std::string region = message["region"];
    std::string subject = "AWS health information";
    std::string account = message["account"];

    localJson["text"] = "*" + subject + "*";
    localJson["channel"] = channel();
    localJson["blocks"] = nljson::array();

    if (message.contains("detail")) {
        nlohmann::json detail = message["detail"];
        std::string eventTypeCode = detail["eventTypeCode"];

        if (!skipEventType(eventTypeCode)) {
            std::vector<std::string> data = message["resources"].get<std::vector<std::string>>();
            std::string resources = data.empty() ? "--" : std::accumulate(data.begin(), data.end(), std::string(), [](const std::string &a, const std::string &b) -> std::string {
                return a + (a.length() > 0 ? "," : "") + b;
            });
            std::string eventArn = detail["eventArn"];

            std::string startTime = detail["startTime"];
            std::string endTime = detail.contains("endTime") ? detail["endTime"] : "--";
            std::string eventTypeCategory = detail["eventTypeCategory"];
            std::string service = detail["service"];
            std::string affectedAccount = detail["affectedAccount"];  // my differ with account if in org

            // affectedEntities is an array

            localJson["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", headerTitle(service, eventTypeCategory, region, account)}}}}));

            auto el = nljson::array();
            el.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Event code*: " + eventTypeCode}}));
            localJson["blocks"].push_back(nljson::object({{"type", "context"}, {"elements", el}}));

            el = nljson::array();
            el.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Affected account*: " + affectedAccount}}));
            el.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Region*: " + region}}));
            localJson["blocks"].push_back(nljson::object({{"type", "context"}, {"elements", el}}));

            el = nljson::array();
            el.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Start time*: " + startTime}}));
            el.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*End time*: " + endTime}}));
            localJson["blocks"].push_back(nljson::object({{"type", "context"}, {"elements", el}}));

            el = nljson::array();
            el.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Affected resources*: " + resources}}));
            localJson["blocks"].push_back(nljson::object({{"type", "context"}, {"elements", el}}));

            auto desc = getDescription(detail["eventDescription"]);
            localJson["blocks"]
                .push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", desc}}}}));

            std::string url = "https://health.aws.amazon.com/health/home#/account/event-log?eventID=" + eventArn;
            auto element = nljson::array();
            auto linkElement = nljson::array();
            linkElement.push_back(nljson::object({{"type", "link"}, {"text", "AWS health link"}, {"url", url}}));
            element.push_back(nljson::object({{"type", "rich_text_section"}, {"elements", linkElement}}));
            localJson["blocks"].push_back(nljson::object({{"type", "rich_text"}, {"elements", element}}));

            json = localJson;
        }
    }
}
const std::string Awshealth::headerTitle(std::string &service, std::string &eventTypeCategory, std::string &region, std::string &account) {
    std::string name = accountName.has_value() ? accountName.value() : account;
    return service + " " + eventTypeCategory + " in region " + region + "(" + name + ")";
}
std::optional<std::string> Awshealth::message() {
    return json.has_value() ? json.value().dump() : std::optional<std::string>{};
}

std::string Awshealth::awsAccount(std::string accountID) {
    return accountID;
}

std::string Awshealth::getDescription(nlohmann::json::array_t desc) {
    auto it = std::find_if(begin(desc), end(desc), [](auto el) {
        return static_cast<std::string>(el["language"]).rfind("en") == 0;
    });

    return std::end(desc) == it ? "" : (*it)["latestDescription"];
}

bool Awshealth::skipEventType(std::string type) {
    if ((std::find(begin(eventTypes), end(eventTypes), type) != std::end(eventTypes))) {
        return true;
    } else {
        return false;
    }
}