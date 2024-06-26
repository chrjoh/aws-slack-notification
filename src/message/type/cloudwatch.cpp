#include "cloudwatch.hpp"

Cloudwatch::Cloudwatch(std::string const &slackChannel, std::optional<std::string> accountName) : Slack(slackChannel, accountName) {
}

void Cloudwatch::parse(nlohmann::json const &message) {
    using nljson = nlohmann::json;
    nljson localJson;
    std::string region = message["Region"];
    std::string account = message["AWSAccountId"];
    std::string subject = "AWS CloudWatch Notification";
    std::string alarmName = message["AlarmName"];
    std::string oldState = message["OldStateValue"];
    std::string newState = message["NewStateValue"];
    std::string alarmDescription = message.contains("AlarmDescription") && !message["AlarmDescription"].is_null() ? message["AlarmDescription"] : "";

    std::string triggerValue;
    if (message.contains("Trigger")) {
        nljson trigger = message["Trigger"];
        std::string metricName = trigger["MetricName"];
        float thresHold = trigger["Threshold"];
        int evaluationPeriods = trigger["EvaluationPeriods"];
        int period = trigger["Period"];
        std::string statistic = trigger["Statistic"];
        std::string comparisonOperator = trigger["ComparisonOperator"];
        triggerValue = statistic + " " + metricName + " " + comparisonOperator + " " + std::to_string(thresHold) + " for " + std::to_string(evaluationPeriods) + " period(s) of " + std::to_string(period) + " seconds.";
    }
    auto awsRegion = regionFromAlarmArn(message["AlarmArn"]);
    char *escapedAlarmName = curl_easy_escape(NULL, alarmName.c_str(), alarmName.size());
    char *escapedRegion = curl_easy_escape(NULL, awsRegion.c_str(), awsRegion.size());
    std::string alarmLink = "https://console.aws.amazon.com/cloudwatch/home?region=" + std::string(escapedRegion) + "#alarm:alarmFilter=ANY;name=" + std::string(escapedAlarmName);
    curl_free(escapedAlarmName);
    curl_free(escapedRegion);

    std::string color = "#FF8000";
    if (newState == "ALARM") {
        color = "#FF0000";
    } else if (newState == "OK") {
        color = "#00FF00";
    }
    std::string headerSubject = headerTitle(subject, region, account);
    localJson["text"] = "*" + subject + "*";
    localJson["channel"] = channel();
    localJson["attachments"] = nljson::array();
    localJson["attachments"].push_back(nljson::object({{"color", color}}));
    localJson["attachments"][0]["blocks"] = nljson::array();
    localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", headerSubject}}}}));
    localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Alarm Name*\n" + alarmName}}}}));
    localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Alarm Description*\n" + alarmDescription}}}}));
    localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Alarm Trigger*\n" + triggerValue}}}}));

    auto fields = nljson::array();
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Old state*\n" + oldState}}));
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Current state*\n" + newState}}));
    localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "section"}, {"fields", fields}}));

    auto element = nljson::array();
    auto linkElement = nljson::array();
    linkElement.push_back(nljson::object({{"type", "link"}, {"text", "Link to Alarm"}, {"url", alarmLink}}));
    element.push_back(nljson::object({{"type", "rich_text_section"}, {"elements", linkElement}}));
    localJson["attachments"][0]["blocks"].push_back(nljson::object({{"type", "rich_text"}, {"elements", element}}));
    json = localJson;
}

std::optional<std::string> Cloudwatch::message() {
    return json.has_value() ? json.value().dump() : std::optional<std::string>{};
}

Cloudwatch::~Cloudwatch() {
}
// private methods

char *Cloudwatch::currentTime() {
    std::time_t currentTime = std::time(nullptr);
    return std::asctime(std::localtime(&currentTime));
}

std::string Cloudwatch::regionFromAlarmArn(std::string s) {
    std::vector<std::string> res;
    int pos_start = 0, pos_end, delim_len = 1;
    std::string token;
    while ((pos_end = s.find(":", pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        // remove white spaces
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    // get the last entry and handle trailing delimeter
    token = s.substr(pos_start);
    token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
    if (token.size() > 0) {
        res.push_back(token);
    }
    return res[3];
}
