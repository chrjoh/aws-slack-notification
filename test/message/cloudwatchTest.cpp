#include <nlohmann/json.hpp>

#include "cloudwatch.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(Cloudwatch, parse_message_to_block_style) {
    Cloudwatch cloudwatch{"slack_channel", std::optional<std::string>{}};
    char time[] = {'t', 'e', 's', 't', '\n'};
    std::string msg = "{\"AlarmName\":\"AlarmForRBRule_project-general-alb-project_alb-project-wafbase-rate-limit-rule_BLOCK\",\"AlarmDescription\":\"Alarm for Rate-based rule events detected on Rule alb-project-wafbase-rate-limit-rule\",\"AWSAccountId\":\"XXXXXXXXXXX\",\"AlarmConfigurationUpdatedTimestamp\":\"2023-02-23T12:07:38.685+0000\",\"NewStateValue\":\"ALARM\",\"NewStateReason\":\"Threshold Crossed: 1 out of the last 20 datapoints [613.0 (16/03/24 12:15:00)] was greater than or equal to the threshold (1.0) and 19 missing datapoints were treated as [NonBreaching] (minimum 1 datapoint for OK -> ALARM transition).\",\"StateChangeTime\":\"2024-03-16T12:16:45.770+0000\",\"Region\":\"EU (Ireland)\",\"AlarmArn\":\"arn:aws:cloudwatch:eu-west-1:XXXXXXXXXXX:alarm:AlarmForRBRule_project-general-alb-project_alb-project-wafbase-rate-limit-rule_BLOCK\",\"OldStateValue\":\"OK\",\"OKActions\":[],\"AlarmActions\":[\"arn:aws:sns:eu-west-1:XXXXXXXXXXX:aws-alerts\"],\"InsufficientDataActions\":[],\"Trigger\":{\"MetricName\":\"BlockedRequests\",\"Namespace\":\"AWS/WAFV2\",\"StatisticType\":\"Statistic\",\"Statistic\":\"SUM\",\"Unit\":null,\"Dimensions\":[{\"value\":\"project-general-alb-project\",\"name\":\"WebACL\"},{\"value\":\"eu-west-1\",\"name\":\"Region\"},{\"value\":\"alb-project-wafbase-rate-limit-rule\",\"name\":\"Rule\"}],\"Period\":60,\"EvaluationPeriods\":20,\"DatapointsToAlarm\":1,\"ComparisonOperator\":\"GreaterThanOrEqualToThreshold\",\"Threshold\":1.0,\"TreatMissingData\":\"notBreaching\",\"EvaluateLowSampleCountPercentile\":\"\"}}";
    std::string expected = "{\"attachments\":[{\"blocks\":[{\"text\":{\"text\":\"AWS CloudWatch Notification in region EU (Ireland)(XXXXXXXXXXX)\",\"type\":\"plain_text\"},\"type\":\"header\"},{\"text\":{\"text\":\"*Alarm Name*\\nAlarmForRBRule_project-general-alb-project_alb-project-wafbase-rate-limit-rule_BLOCK\",\"type\":\"mrkdwn\"},\"type\":\"section\"},{\"text\":{\"text\":\"*Alarm Description*\\nAlarm for Rate-based rule events detected on Rule alb-project-wafbase-rate-limit-rule\",\"type\":\"mrkdwn\"},\"type\":\"section\"},{\"text\":{\"text\":\"*Alarm Trigger*\\nSUM BlockedRequests GreaterThanOrEqualToThreshold 1.000000 for 20 period(s) of 60 seconds.\",\"type\":\"mrkdwn\"},\"type\":\"section\"},{\"fields\":[{\"text\":\"*Old state*\\nOK\",\"type\":\"mrkdwn\"},{\"text\":\"*Current state*\\nALARM\",\"type\":\"mrkdwn\"}],\"type\":\"section\"},{\"elements\":[{\"elements\":[{\"text\":\"Link to Alarm\",\"type\":\"link\",\"url\":\"https://console.aws.amazon.com/cloudwatch/home?region=EU%20%28Ireland%29#alarm:alarmFilter=ANY;name=AlarmForRBRule_project-general-alb-project_alb-project-wafbase-rate-limit-rule_BLOCK\"}],\"type\":\"rich_text_section\"}],\"type\":\"rich_text\"}],\"color\":\"#FF0000\"}],\"channel\":\"slack_channel\",\"text\":\"*AWS CloudWatch Notification*\"}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    cloudwatch.parse(jsonMessage);
    auto result = cloudwatch.message();
    ASSERT_EQ(result.value(), expected);
}

TEST(Cloudwatch, Should_return_Cloudwatch_block_style_messge_with_danger_color) {
    std::string msg = "{\"AlarmName\":\"AlarmForRBRule_project-general-alb-project_alb-project-wafbase-rate-limit-rule_BLOCK\",\"AlarmDescription\":\"Alarm for Rate-based rule events detected on Rule alb-project-wafbase-rate-limit-rule\",\"AWSAccountId\":\"XXXXXXXXXXX\",\"AlarmConfigurationUpdatedTimestamp\":\"2023-02-23T12:07:38.685+0000\",\"NewStateValue\":\"ALARM\",\"NewStateReason\":\"Threshold Crossed: 1 out of the last 20 datapoints [613.0 (16/03/24 12:15:00)] was greater than or equal to the threshold (1.0) and 19 missing datapoints were treated as [NonBreaching] (minimum 1 datapoint for OK -> ALARM transition).\",\"StateChangeTime\":\"2024-03-16T12:16:45.770+0000\",\"Region\":\"EU (Ireland)\",\"AlarmArn\":\"arn:aws:cloudwatch:eu-west-1:XXXXXXXXXXX:alarm:AlarmForRBRule_project-general-alb-project_alb-project-wafbase-rate-limit-rule_BLOCK\",\"OldStateValue\":\"OK\",\"OKActions\":[],\"AlarmActions\":[\"arn:aws:sns:eu-west-1:XXXXXXXXXXX:aws-alerts\"],\"InsufficientDataActions\":[],\"Trigger\":{\"MetricName\":\"BlockedRequests\",\"Namespace\":\"AWS/WAFV2\",\"StatisticType\":\"Statistic\",\"Statistic\":\"SUM\",\"Unit\":null,\"Dimensions\":[{\"value\":\"project-general-alb-project\",\"name\":\"WebACL\"},{\"value\":\"eu-west-1\",\"name\":\"Region\"},{\"value\":\"alb-project-wafbase-rate-limit-rule\",\"name\":\"Rule\"}],\"Period\":60,\"EvaluationPeriods\":20,\"DatapointsToAlarm\":1,\"ComparisonOperator\":\"GreaterThanOrEqualToThreshold\",\"Threshold\":1.0,\"TreatMissingData\":\"notBreaching\",\"EvaluateLowSampleCountPercentile\":\"\"}}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Cloudwatch cloudwatch{"slack_channel", std::optional<std::string>{}};
    cloudwatch.parse(jsonMessage);
    auto result = cloudwatch.message();
    ASSERT_THAT(result.value(), testing::HasSubstr("#FF0000"));
}

TEST(Cloudwatch, Should_return_Cloudwatch_block_style_messge_with_OK_color) {
    std::string msg = "{\"AlarmName\":\"package-general-live-event-filter-errors\",\"AlarmDescription\":\"The TV4 live sports event filter lambda has had atleast 1 error within the last 5 minutes!\",\"AWSAccountId\":\"123456712345\",\"AlarmConfigurationUpdatedTimestamp\":\"2023-08-17T08:47:29.454+0000\",\"NewStateValue\":\"OK\",\"NewStateReason\":\"Threshold Crossed: 1 datapoint [0.0 (19/03/24 17:14:00)] was not greater than or equal to the threshold (1.0).\",\"StateChangeTime\":\"2024-03-19T17:19:14.422+0000\",\"Region\":\"EU (Ireland)\",\"AlarmArn\":\"arn:aws:cloudwatch:eu-west-1:123456712345:alarm:package-general-live-event-filter-errors\",\"OldStateValue\":\"ALARM\",\"OKActions\":[\"arn:aws:sns:eu-west-1:123456712345:aws-alerts\"],\"AlarmActions\":[\"arn:aws:sns:eu-west-1:123456712345:aws-alerts\"],\"InsufficientDataActions\":[],\"Trigger\":{\"MetricName\":\"Errors\",\"Namespace\":\"AWS/Lambda\",\"StatisticType\":\"Statistic\",\"Statistic\":\"SUM\",\"Unit\":null,\"Dimensions\":[{\"value\":\"package-general-live-event-filter-function\",\"name\":\"FunctionName\"}],\"Period\":300,\"EvaluationPeriods\":1,\"ComparisonOperator\":\"GreaterThanOrEqualToThreshold\",\"Threshold\":1.0,\"TreatMissingData\":\"notBreaching\",\"EvaluateLowSampleCountPercentile\":\"\"}}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Cloudwatch cloudwatch{"slack_channel", std::optional<std::string>{}};
    cloudwatch.parse(jsonMessage);
    auto result = cloudwatch.message();
    ASSERT_THAT(result.value(), testing::HasSubstr("#00FF00"));
}
