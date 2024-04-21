#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "client.hpp"
#include "var.hpp"

class Environment {
   public:
    Environment(std::shared_ptr<HttpClient> httpclient);
    Environment(){};
    std::vector<std::string> getStrings(std::string s, std::string del);
    // virtual ~Environment();
    virtual std::string OAuthTokenName() const;
    virtual std::string AWSSessionToken() const;
    virtual std::string slackChannelName() const;
    virtual std::string slackOAuthToken() const;
    virtual std::optional<std::string> accountName() const;
    virtual bool onlyLatestImageTag() const;
    virtual double inspcetorThresHold() const;
    std::vector<std::string> eventsToSkip() const;

   protected:
    std::unique_ptr<Var<std::string>> oAuthKey;
    std::unique_ptr<Var<std::string>> slackChannel;
    std::unique_ptr<Var<std::string>> awsSessionToken;
    std::unique_ptr<Var<std::string>> awsAccountName;
    std::unique_ptr<Var<std::string>> awsHealthEnventsToSkip;
    std::unique_ptr<Var<bool>> onlyLatest;
    std::unique_ptr<Var<double>> thresHold;
    std::vector<std::string> eventTypes;

   private:
    std::shared_ptr<HttpClient> httpclient;
    std::string slackToken;
    std::string const parameterUrl = "http://localhost:2773/systemsmanager/parameters/get";
};

// Environment::~Environment() {
// }
