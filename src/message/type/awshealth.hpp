#include <algorithm>
#include <nlohmann/json.hpp>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "slack.hpp"

class Awshealth : public Slack {
   public:
    Awshealth(std::string const &slackChannel, std::optional<std::string> accountName, std::vector<std::string> types = {});
    void parse(nlohmann::json const &message);

    std::optional<std::string> message();
    ~Awshealth() = default;
    bool skipEventType(std::string_view type) const;

   private:
    std::vector<std::string> eventTypes;

    std::string getDescription(nlohmann::json::array_t const &desc) const;
};
