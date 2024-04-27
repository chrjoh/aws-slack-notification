#include "inspector.hpp"

Inspector::Inspector(bool const &onlyLatest, double const &serverityThreshold, std::string const &slackChannel, std::optional<std::string> accountName)
    : onlyLatest(onlyLatest), serverityThreshold(serverityThreshold), Slack(slackChannel, accountName) {
}

void Inspector::parse(nlohmann::json const &message) {
    auto resourceType = typeOffinding(message);
    switch (resourceType) {
        case FindingType::AWS_ECR_CONTAINER_IMAGE:
            parseECRMessage(message);
            break;
        case FindingType::AWS_LAMBDA_FUNCTION:
            parseLambdaMessage(message);
            break;
        case FindingType::INITIAL_SCAN_COMPLETE:
            parseInitialScanMessage(message);
            break;
        default:
            break;
    }
}

std::optional<std::string> Inspector::message() {
    return json.has_value() ? json.value().dump() : std::optional<std::string>{};
}

Inspector::FindingType Inspector::typeOffinding(nlohmann::json const &message) {
    if (hasFindingResources(message)) {
        if (message["detail"]["resources"][0]["type"] == "AWS_LAMBDA_FUNCTION") {
            return FindingType::AWS_LAMBDA_FUNCTION;
        } else if (message["detail"]["resources"][0]["type"] == "AWS_ECR_CONTAINER_IMAGE") {
            return FindingType::AWS_ECR_CONTAINER_IMAGE;
        }
    } else if (message.contains("detail") && message["detail"].contains("scan-status") && message["detail"]["scan-status"] == "INITIAL_SCAN_COMPLETE") {
        return FindingType::INITIAL_SCAN_COMPLETE;
    }
    return FindingType::UNKNOWN;
}

bool Inspector::hasFindingResources(const nlohmann::json &message) {
    return message.contains("detail") && message["detail"].contains("resources") && message["detail"]["resources"].size() > 0;
}

void Inspector::parseInitialScanMessage(nlohmann::json const &message) {
    using nljson = nlohmann::json;
    nljson localJson;
    bool haveLatestTag = true;
    std::string detailType = message["detail-type"];
    std::string region = message["region"];
    std::string account = message["account"];
    std::string resource = message["resources"][0];
    auto detail = message["detail"];
    localJson["text"] = "*" + detailType + " in " + region + "*";
    localJson["channel"] = channel();
    localJson["blocks"] = nljson::array();

    auto tags = detail["image-tags"].get<std::vector<std::string>>();
    if ((std::find(begin(tags), end(tags), "latest") == std::end(tags))) {
        haveLatestTag = false;
    }

    if (!onlyLatest || (onlyLatest && haveLatestTag)) {
        std::string subject = detailType;
        std::string headerSubject = headerTitle(subject, region, account);
        std::string repositoryName = detail["repository-name"];
        auto findinfSeveritys = detail["finding-severity-counts"];
        localJson["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", headerSubject}}}}));
        localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Repository*\n`" + repositoryName + "`"}}}}));

        auto fields = nljson::array();
        long critical = findinfSeveritys["CRITICAL"];
        long high = findinfSeveritys["HIGH"];
        long medium = findinfSeveritys["MEDIUM"];
        long total = findinfSeveritys["TOTAL"];
        fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*CRITICAL*\n" + std::to_string(critical)}}));
        fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*HIGH*\n" + std::to_string(high)}}));
        fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*MEDIUM*\n" + std::to_string(medium)}}));
        fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*TOTAL*\n" + std::to_string(total)}}));
        localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"fields", fields}}));
        json = localJson;
    }
}
void Inspector::parseECRMessage(nlohmann::json const &message) {
    using nljson = nlohmann::json;
    nljson localJson;
    bool haveLatestTag = true;
    auto detail = message["detail"];

    auto tags = detail["resources"][0]["details"]["awsEcrContainerImage"]["imageTags"].get<std::vector<std::string>>();
    if ((std::find(begin(tags), end(tags), "latest") == std::end(tags))) {
        haveLatestTag = false;
    }
    Inspector::Message extracted{message};
    extracted.subject = extracted.detailType + " for ECR image";

    if (extracted.score >= serverityThreshold) {
        if (onlyLatest && !haveLatestTag) {
            json = {};
        } else {
            createECRorLambdaSlackMessage(localJson, extracted);
            json = localJson;
        }
    }
}

void Inspector::parseLambdaMessage(nlohmann::json const &message) {
    using nljson = nlohmann::json;
    nljson localJson;

    Inspector::Message extracted{message};
    extracted.subject = extracted.detailType + " for lambda function";

    if (extracted.score >= serverityThreshold) {
        createECRorLambdaSlackMessage(localJson, extracted);
        json = localJson;
    }
}

void Inspector::createECRorLambdaSlackMessage(nlohmann::json &localJson, Inspector::Message const &extracted) {
    using nljson = nlohmann::json;
    localJson["text"] = "*" + extracted.detailType + " in " + extracted.region + "*";
    localJson["channel"] = channel();
    localJson["blocks"] = nljson::array();
    std::string headerSubject = headerTitle(extracted.subject, extracted.region, extracted.account);
    localJson["blocks"].push_back(nljson::object({{"type", "header"}, {"text", {{"type", "plain_text"}, {"text", headerSubject}}}}));

    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Description*\n" + extracted.description}}}}));
    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Vulnerability*\n" + extracted.vulnerability}}}}));
    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"text", {{"type", "mrkdwn"}, {"text", "*Resource*\n`" + extracted.resource + "`"}}}}));

    auto fields = nljson::array();
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Exploit available*\n" + extracted.exploitAvailable}}));
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Fix Available*\n" + extracted.fixAvailable}}));
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Severity*\n" + extracted.severity}}));
    fields.push_back(nljson::object({{"type", "mrkdwn"}, {"text", "*Inspector score*\n" + extracted.inspectorScore}}));
    localJson["blocks"].push_back(nljson::object({{"type", "section"}, {"fields", fields}}));
}

Inspector::~Inspector() {
}

Inspector::Message::Message(nlohmann::json const &message) {
    detailType = message["detail-type"];
    region = message["region"];
    account = message["account"];
    resource = message["resources"][0];
    auto detail = message["detail"];
    description = detail["description"];
    exploitAvailable = detail["exploitAvailable"];
    fixAvailable = detail["fixAvailable"];
    inspectorScore = "--";
    if (detail.contains("inspectorScore")) {
        score = detail["inspectorScore"];
        std::stringstream s;
        s << std::setprecision(2) << score;
        inspectorScore = s.str();
    }
    severity = detail["severity"];
    vulnerability = detail["title"];
}
