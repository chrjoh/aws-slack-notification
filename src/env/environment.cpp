#include "environment.hpp"

Environment::Environment(std::shared_ptr<HttpClient> httpclient) : httpclient(httpclient) {
    oAuthKey = std::make_unique<Var<std::string>>("OAUTH_TOKEN_KEY");
    slackChannel = std::make_unique<Var<std::string>>("SLACK_CHANNEL");
    awsSessionToken = std::make_unique<Var<std::string>>("AWS_SESSION_TOKEN");
    thresHold = std::make_unique<Var<double>>("THRESHOLD", 0.0);
    onlyLatest = std::make_unique<Var<bool>>("ONLYLATEST", false);
    awsAccountName = std::make_unique<Var<std::string>>("AWS_ACCOUNT_NAME", "");
    awsHealthEnventsToSkip = std::make_unique<Var<std::string>>("AWS_HEALTH_EVENTS_TO_SKIP", "");
    if (awsHealthEnventsToSkip->get().size() > 0) {
        eventTypes = getStrings(awsHealthEnventsToSkip->get(), ",");
    }
    auto Doc = httpclient->doGetSecretParameter(parameterUrl, AWSSessionToken(), Params{{"withDecryption", "true"}, {"name", OAuthTokenName()}});
    if (Doc.contains("Parameter") && Doc["Parameter"].contains("Value")) {
        slackToken = Doc["Parameter"]["Value"];
    } else {
        std::cerr << "Failed to fetch SSM parameter from localhost" << std::endl;
        throw(std::domain_error("Failed to fetch SSM parameter"));
    }
}
std::string Environment::OAuthTokenName() const {
    return oAuthKey->get();
}

std::string Environment::AWSSessionToken() const {
    return awsSessionToken->get();
}

std::string Environment::slackOAuthToken() const {
    return slackToken;
}

std::optional<std::string> Environment::accountName() const {
    if (awsAccountName->get() == "") {
        return std::optional<std::string>{};
    } else {
        return awsAccountName->get();
    }
}

std::string Environment::slackChannelName() const {
    return slackChannel->get();
}

bool Environment::onlyLatestImageTag() const {
    return onlyLatest->get();
}

double Environment::inspcetorThresHold() const {
    return thresHold->get();
}

std::vector<std::string> Environment::eventsToSkip() const {
    return eventTypes;
}

std::vector<std::string> Environment::getStrings(std::string s, std::string del) {
    std::vector<std::string> res;
    int pos_start = 0, pos_end, delim_len = del.length();
    std::string token;
    while ((pos_end = s.find(del, pos_start)) != std::string::npos) {
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
    return res;
}