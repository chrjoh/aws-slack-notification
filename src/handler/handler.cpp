#include "handler.hpp"

Handler::Handler(std::shared_ptr<HttpClient> httpclient, Environment &env) : httpclient(httpclient), env(std::move(env)) {
}

void Handler::process(std::string const &payload) {
    auto slack = parseAndCreateSlackMessage(payload, env.slackChannelName());
    if (slack->message().has_value()) {
        httpclient->doPostToSlack(slackUrl, env.slackOAuthToken(), slack->message().value());
    }
}

std::unique_ptr<Slack> Handler::parseAndCreateSlackMessage(std::string const &payload, std::string const &slackChannel) {
    nlohmann::json json;
    try {
        auto data = json.parse(payload, nullptr, true);
        if (data.contains("source") && data["source"] == "aws.guardduty") {
            auto guardDuty = std::make_unique<Guardduty>(slackChannel);
            guardDuty->parse(data);
            return guardDuty;
        } else if (data.contains("source") && data["source"] == "aws.inspector2") {
            auto inspector = std::make_unique<Inspector>(env.onlyLatestImageTag(), env.inspcetorThresHold(), slackChannel);
            inspector->parse(data);
            return inspector;
        } else if (data.contains("source") && data["source"] == "aws.health") {
            auto awsHealth = std::make_unique<Awshealth>(slackChannel, env.accountName(), env.eventsToSkip());
            awsHealth->parse(data);
            return awsHealth;
        } else if (haveSnSMessage(data)) {
            std::string SnSmessage = data["Records"][0]["Sns"]["Message"];
            auto message = json.parse(SnSmessage);
            if (message.contains("AlarmName")) {
                auto cloudwatch = std::make_unique<Cloudwatch>(slackChannel);
                cloudwatch->parse(message);
                return cloudwatch;
            }
        }

    } catch (const nlohmann::json::parse_error &ex) {
        std::cerr << "Failed to construct slack message: \n"
                  << ex.what() << std::endl;
        return std::make_unique<Slack>(slackChannel, true);
    }
    return std::make_unique<Slack>(slackChannel);
}

Handler::~Handler() {
}
// private methods
bool Handler::haveSnSMessage(nlohmann::json &data) {
    return data.contains("Records") && data["Records"].size() > 0 && data["Records"][0].contains("Sns");
}
