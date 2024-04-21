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

    std::optional<std::string> message();
    ~Awshealth() = default;
    bool skipEventType(std::string const &type) const;

   private:
    std::vector<std::string> eventTypes;

    std::string awsAccount(std::string const &accountID) const;
    std::string getDescription(nlohmann::json::array_t const &desc) const;
};
