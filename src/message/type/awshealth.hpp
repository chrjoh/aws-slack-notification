#include <algorithm>
#include <nlohmann/json.hpp>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

#include "slack.hpp"

class Awshealth : public Slack {
   public:
    Awshealth(std::string const &slackChannel, std::optional<std::string> accountName, std::vector<std::string> types = {});
    void parse(nlohmann::json const &message);
    const std::string headerTitle(std::string &service, std::string &eventTypeCategory, std::string &region, std::string &account);
    std::optional<std::string> message();
    ~Awshealth() = default;
    bool skipEventType(std::string type);

   private:
    std::vector<std::string> eventTypes;
    std::optional<std::string> accountName;
    std::string awsAccount(std::string accountID);
    std::string getDescription(nlohmann::json::array_t desc);
};
