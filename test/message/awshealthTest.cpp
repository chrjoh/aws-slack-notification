#include <nlohmann/json.hpp>

#include "awshealth.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(Awshealth, parse_message_to_block_style) {
    std::string msg = "{\n          \"version\": \"0\",\n          \"id\": \"7bf73129-1428-4cd3-a780-95db273d1602\",\n          \"detail-type\": \"AWS Health Event\",\n          \"source\": \"aws.health\",\n          \"account\": \"123456789012\",\n          \"time\": \"2023-01-27T09:01:22Z\",\n          \"region\": \"af-south-1\",\n          \"resources\": [],\n          \"detail\": {\n            \"eventArn\": \"arn:aws:health:af-south-1::event/EC2/AWS_EC2_OPERATIONAL_ISSUE/AWS_EC2_OPERATIONAL_ISSUE_7f35c8ae-af1f-54e6-a526-d0179ed6d68f\",\n            \"service\": \"EC2\",\n            \"eventTypeCode\": \"AWS_EC2_OPERATIONAL_ISSUE\",\n            \"eventTypeCategory\": \"issue\",\n            \"eventScopeCode\": \"PUBLIC\",\n            \"communicationId\": \"01b0993207d81a09dcd552ebd1e633e36cf1f09a-1\",\n            \"startTime\": \"Fri, 27 Jan 2023 06:02:51 GMT\",\n            \"endTime\": \"Fri, 27 Jan 2023 09:01:22 GMT\",\n            \"lastUpdatedTime\": \"Fri, 27 Jan 2023 09:01:22 GMT\",\n            \"statusCode\": \"open\",\n            \"eventRegion\": \"af-south-1\",\n            \"eventDescription\": \n            [{\n              \"language\": \"en_US\",\n              \"latestDescription\": \"Current severity level: Operating normally\\n\\n[RESOLVED] \\n\\n [03:15 PM PST] We continue see recovery \\n\\nThe following AWS services were previously impacted but are now operating normally: APPSYNC, BACKUP, EVENTS.\\n[1] https://aws.amazon.com/support\"      \n            }],\n            \"affectedEntities\":[],\n            \"page\": \"1\",\n            \"totalPages\": \"1\",\n            \"affectedAccount\": \"123456789012\"\n          }\n        }";
    std::string expected = "{\"blocks\":[{\"text\":{\"text\":\"EC2 issue in region af-south-1(123456789012)\",\"type\":\"plain_text\"},\"type\":\"header\"},{\"elements\":[{\"text\":\"*Event code*: AWS_EC2_OPERATIONAL_ISSUE\",\"type\":\"mrkdwn\"}],\"type\":\"context\"},{\"elements\":[{\"text\":\"*Affected account*: 123456789012\",\"type\":\"mrkdwn\"},{\"text\":\"*Region*: af-south-1\",\"type\":\"mrkdwn\"}],\"type\":\"context\"},{\"elements\":[{\"text\":\"*Start time*: Fri, 27 Jan 2023 06:02:51 GMT\",\"type\":\"mrkdwn\"},{\"text\":\"*End time*: Fri, 27 Jan 2023 09:01:22 GMT\",\"type\":\"mrkdwn\"}],\"type\":\"context\"},{\"elements\":[{\"text\":\"*Affected resources*: --\",\"type\":\"mrkdwn\"}],\"type\":\"context\"},{\"text\":{\"text\":\"Current severity level: Operating normally\\n\\n[RESOLVED] \\n\\n [03:15 PM PST] We continue see recovery \\n\\nThe following AWS services were previously impacted but are now operating normally: APPSYNC, BACKUP, EVENTS.\\n[1] https://aws.amazon.com/support\",\"type\":\"mrkdwn\"},\"type\":\"section\"},{\"elements\":[{\"elements\":[{\"text\":\"AWS health link\",\"type\":\"link\",\"url\":\"https://health.aws.amazon.com/health/home#/account/event-log?eventID=arn:aws:health:af-south-1::event/EC2/AWS_EC2_OPERATIONAL_ISSUE/AWS_EC2_OPERATIONAL_ISSUE_7f35c8ae-af1f-54e6-a526-d0179ed6d68f\"}],\"type\":\"rich_text_section\"}],\"type\":\"rich_text\"}],\"channel\":\"slack_channel\",\"text\":\"*AWS health information*\"}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Awshealth awsHealth{"slack_channel"};
    awsHealth.parse(jsonMessage);
    auto result = awsHealth.message();
    ASSERT_EQ(result.value(), expected);
}
TEST(Awshealth, parse_message_to_block_style_with_no_endtime) {
    std::string msg = "{\n          \"version\": \"0\",\n          \"id\": \"7bf73129-1428-4cd3-a780-95db273d1602\",\n          \"detail-type\": \"AWS Health Event\",\n          \"source\": \"aws.health\",\n          \"account\": \"123456789012\",\n          \"time\": \"2023-01-27T09:01:22Z\",\n          \"region\": \"af-south-1\",\n          \"resources\": [],\n          \"detail\": {\n            \"eventArn\": \"arn:aws:health:af-south-1::event/EC2/AWS_EC2_OPERATIONAL_ISSUE/AWS_EC2_OPERATIONAL_ISSUE_7f35c8ae-af1f-54e6-a526-d0179ed6d68f\",\n            \"service\": \"EC2\",\n            \"eventTypeCode\": \"AWS_EC2_OPERATIONAL_ISSUE\",\n            \"eventTypeCategory\": \"issue\",\n            \"eventScopeCode\": \"PUBLIC\",\n            \"communicationId\": \"01b0993207d81a09dcd552ebd1e633e36cf1f09a-1\",\n            \"startTime\": \"Fri, 27 Jan 2023 06:02:51 GMT\",\n            \"lastUpdatedTime\": \"Fri, 27 Jan 2023 09:01:22 GMT\",\n            \"statusCode\": \"open\",\n            \"eventRegion\": \"af-south-1\",\n            \"eventDescription\": \n            [{\n              \"language\": \"en_US\",\n              \"latestDescription\": \"Current severity level: Operating normally\\n\\n[RESOLVED] \\n\\n [03:15 PM PST] We continue see recovery \\n\\nThe following AWS services were previously impacted but are now operating normally: APPSYNC, BACKUP, EVENTS.\\n[1] https://aws.amazon.com/support\"      \n            }],\n            \"affectedEntities\":[],\n            \"page\": \"1\",\n            \"totalPages\": \"1\",\n            \"affectedAccount\": \"123456789012\"\n          }\n        }";
    std::string expected = "{\"blocks\":[{\"text\":{\"text\":\"EC2 issue in region af-south-1(123456789012)\",\"type\":\"plain_text\"},\"type\":\"header\"},{\"elements\":[{\"text\":\"*Event code*: AWS_EC2_OPERATIONAL_ISSUE\",\"type\":\"mrkdwn\"}],\"type\":\"context\"},{\"elements\":[{\"text\":\"*Affected account*: 123456789012\",\"type\":\"mrkdwn\"},{\"text\":\"*Region*: af-south-1\",\"type\":\"mrkdwn\"}],\"type\":\"context\"},{\"elements\":[{\"text\":\"*Start time*: Fri, 27 Jan 2023 06:02:51 GMT\",\"type\":\"mrkdwn\"},{\"text\":\"*End time*: --\",\"type\":\"mrkdwn\"}],\"type\":\"context\"},{\"elements\":[{\"text\":\"*Affected resources*: --\",\"type\":\"mrkdwn\"}],\"type\":\"context\"},{\"text\":{\"text\":\"Current severity level: Operating normally\\n\\n[RESOLVED] \\n\\n [03:15 PM PST] We continue see recovery \\n\\nThe following AWS services were previously impacted but are now operating normally: APPSYNC, BACKUP, EVENTS.\\n[1] https://aws.amazon.com/support\",\"type\":\"mrkdwn\"},\"type\":\"section\"},{\"elements\":[{\"elements\":[{\"text\":\"AWS health link\",\"type\":\"link\",\"url\":\"https://health.aws.amazon.com/health/home#/account/event-log?eventID=arn:aws:health:af-south-1::event/EC2/AWS_EC2_OPERATIONAL_ISSUE/AWS_EC2_OPERATIONAL_ISSUE_7f35c8ae-af1f-54e6-a526-d0179ed6d68f\"}],\"type\":\"rich_text_section\"}],\"type\":\"rich_text\"}],\"channel\":\"slack_channel\",\"text\":\"*AWS health information*\"}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Awshealth awsHealth{"slack_channel"};
    awsHealth.parse(jsonMessage);
    auto result = awsHealth.message();
    ASSERT_EQ(result.value(), expected);
}
TEST(Awshealth, check_if_event_should_be_skipped) {
    std::vector<std::string> data{"key1", "key2", "key3"};
    Awshealth awsHealth{"slack_channel", data};
    EXPECT_FALSE(awsHealth.skipEventType("key4"));
    EXPECT_TRUE(awsHealth.skipEventType("key2"));
}
TEST(Awshealth, skip_event) {
    std::string msg = "{\n          \"version\": \"0\",\n          \"id\": \"7bf73129-1428-4cd3-a780-95db273d1602\",\n          \"detail-type\": \"AWS Health Event\",\n          \"source\": \"aws.health\",\n          \"account\": \"123456789012\",\n          \"time\": \"2023-01-27T09:01:22Z\",\n          \"region\": \"af-south-1\",\n          \"resources\": [],\n          \"detail\": {\n            \"eventArn\": \"arn:aws:health:af-south-1::event/EC2/AWS_EC2_OPERATIONAL_ISSUE/AWS_EC2_OPERATIONAL_ISSUE_7f35c8ae-af1f-54e6-a526-d0179ed6d68f\",\n            \"service\": \"EC2\",\n            \"eventTypeCode\": \"AWS_ACM_RENEWAL_STATE_CHANGE\",\n            \"eventTypeCategory\": \"issue\",\n            \"eventScopeCode\": \"PUBLIC\",\n            \"communicationId\": \"01b0993207d81a09dcd552ebd1e633e36cf1f09a-1\",\n            \"startTime\": \"Fri, 27 Jan 2023 06:02:51 GMT\",\n            \"lastUpdatedTime\": \"Fri, 27 Jan 2023 09:01:22 GMT\",\n            \"statusCode\": \"open\",\n            \"eventRegion\": \"af-south-1\",\n            \"eventDescription\": \n            [{\n              \"language\": \"en_US\",\n              \"latestDescription\": \"Current severity level: Operating normally\\n\\n[RESOLVED] \\n\\n [03:15 PM PST] We continue see recovery \\n\\nThe following AWS services were previously impacted but are now operating normally: APPSYNC, BACKUP, EVENTS.\\n[1] https://aws.amazon.com/support\"      \n            }],\n            \"affectedEntities\":[],\n            \"page\": \"1\",\n            \"totalPages\": \"1\",\n            \"affectedAccount\": \"123456789012\"\n          }\n        }";
    nlohmann::json json;
    std::vector<std::string> skipEvents{"AWS_ACM_RENEWAL_STATE_CHANGE"};
    auto jsonMessage = json.parse(msg);

    Awshealth awsHealth{"slack_channel", skipEvents};
    awsHealth.parse(jsonMessage);
    auto result = awsHealth.message();
    ASSERT_EQ(result.has_value(), false);
}
