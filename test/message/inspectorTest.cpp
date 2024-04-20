#include <nlohmann/json.hpp>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "inspector.hpp"

TEST(Inspector, ensure_corrct_slack_message) {
    std::string msg = "{\"version\":\"0\",\"id\":\"5b52952e-26df-3a51-6d14-4dbe737e58ec\",\"detail-type\":\"Inspector2 Finding\",\"source\":\"aws.inspector2\",\"account\":\"111122223333\",\"time\":\"2023-01-19T21:59:00Z\",\"region\":\"us-east-1\",\"resources\":[\"arn:aws:ecr:us-east-1:111122223333:repository/inspector2/sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\"],\"detail\":{\"awsAccountId\":\"111122223333\",\"description\":\"libcurlwouldreuseapreviouslycreatedconnectionevenwhenaTLSorSSHrelatedoptionhadbeenchangedthatshouldhaveprohibitedreuse.libcurlkeepspreviouslyusedconnectionsinaconnectionpoolforsubsequenttransferstoreuseifoneofthemmatchesthesetup.However,severalTLSandSSHsettingswereleftoutfromtheconfigurationmatchchecks,makingthemmatchtooeasily.\",\"exploitAvailable\":\"NO\",\"findingArn\":\"arn:aws:inspector2:us-east-1:111122223333:finding/FINDING_ID\",\"firstObservedAt\":\"Jan19,2023,9:59:00PM\",\"fixAvailable\":\"YES\",\"inspectorScore\":7.5,\"inspectorScoreDetails\":{\"adjustedCvss\":{\"adjustments\":[],\"cvssSource\":\"NVD\",\"score\":7.5,\"scoreSource\":\"NVD\",\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:H/A:N\",\"version\":\"3.1\"}},\"lastObservedAt\":\"Jan19,2023,9:59:00PM\",\"packageVulnerabilityDetails\":{\"cvss\":[{\"baseScore\":5,\"scoringVector\":\"AV:N/AC:L/Au:N/C:N/I:P/A:N\",\"source\":\"NVD\",\"version\":\"2.0\"},{\"baseScore\":7.5,\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:H/A:N\",\"source\":\"NVD\",\"version\":\"3.1\"}],\"referenceUrls\":[\"https://hackerone.com/reports/1555796\",\"https://security.gentoo.org/glsa/202212-01\",\"https://lists.debian.org/debian-lts-announce/2022/08/msg00017.html\",\"https://www.debian.org/security/2022/dsa-5197\"],\"relatedVulnerabilities\":[],\"source\":\"NVD\",\"sourceUrl\":\"https://nvd.nist.gov/vuln/detail/CVE-2022-27782\",\"vendorCreatedAt\":\"Jun2,2022,2:15:00PM\",\"vendorSeverity\":\"HIGH\",\"vendorUpdatedAt\":\"Jan5,2023,5:51:00PM\",\"vulnerabilityId\":\"CVE-2022-27782\",\"vulnerablePackages\":[{\"arch\":\"X86_64\",\"epoch\":0,\"fixedInVersion\":\"0:7.61.1-22.el8_6.3\",\"name\":\"libcurl\",\"packageManager\":\"OS\",\"release\":\"22.el8\",\"remediation\":\"yumupdatelibcurl\",\"sourceLayerHash\":\"sha256:38a980f2cc8accf69c23deae6743d42a87eb34a54f02396f3fcfd7c2d06e2c5b\",\"version\":\"7.61.1\"},{\"arch\":\"X86_64\",\"epoch\":0,\"fixedInVersion\":\"0:7.61.1-22.el8_6.3\",\"name\":\"curl\",\"packageManager\":\"OS\",\"release\":\"22.el8\",\"remediation\":\"yumupdatecurl\",\"sourceLayerHash\":\"sha256:38a980f2cc8accf69c23deae6743d42a87eb34a54f02396f3fcfd7c2d06e2c5b\",\"version\":\"7.61.1\"}]},\"remediation\":{\"recommendation\":{\"text\":\"NoneProvided\"}},\"resources\":[{\"details\":{\"awsEcrContainerImage\":{\"architecture\":\"amd64\",\"imageHash\":\"sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\",\"imageTags\":[\"o3\"],\"platform\":\"ORACLE_LINUX_8\",\"pushedAt\":\"Jan19,2023,7:38:39PM\",\"registry\":\"111122223333\",\"repositoryName\":\"inspector2\"}},\"id\":\"arn:aws:ecr:us-east-1:111122223333:repository/inspector2/sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\",\"partition\":\"aws\",\"region\":\"us-east-1\",\"type\":\"AWS_ECR_CONTAINER_IMAGE\"}],\"severity\":\"HIGH\",\"status\":\"ACTIVE\",\"title\":\"CVE-2022-27782-libcurl,curl\",\"type\":\"PACKAGE_VULNERABILITY\",\"updatedAt\":\"Jan19,2023,9:59:00PM\"}}";
    std::string expected = "{\"blocks\":[{\"text\":{\"text\":\"Inspector2 Finding for ECR image in us-east-1\",\"type\":\"plain_text\"},\"type\":\"header\"},{\"text\":{\"text\":\"*Description*\\nlibcurlwouldreuseapreviouslycreatedconnectionevenwhenaTLSorSSHrelatedoptionhadbeenchangedthatshouldhaveprohibitedreuse.libcurlkeepspreviouslyusedconnectionsinaconnectionpoolforsubsequenttransferstoreuseifoneofthemmatchesthesetup.However,severalTLSandSSHsettingswereleftoutfromtheconfigurationmatchchecks,makingthemmatchtooeasily.\",\"type\":\"mrkdwn\"},\"type\":\"section\"},{\"text\":{\"text\":\"*Vulnerability*\\nCVE-2022-27782-libcurl,curl\",\"type\":\"mrkdwn\"},\"type\":\"section\"},{\"text\":{\"text\":\"*Resource*\\n`arn:aws:ecr:us-east-1:111122223333:repository/inspector2/sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13`\",\"type\":\"mrkdwn\"},\"type\":\"section\"},{\"fields\":[{\"text\":\"*Exploit available*\\nNO\",\"type\":\"mrkdwn\"},{\"text\":\"*Fix Available*\\nYES\",\"type\":\"mrkdwn\"},{\"text\":\"*Severity*\\nHIGH\",\"type\":\"mrkdwn\"},{\"text\":\"*Inspector score*\\n7.500000\",\"type\":\"mrkdwn\"}],\"type\":\"section\"}],\"channel\":\"slack_channel\",\"text\":\"*Inspector2 Finding in us-east-1*\"}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Inspector inspector{false, 0.0, "slack_channel", std::optional<std::string>{}};
    inspector.parse(jsonMessage);
    auto result = inspector.message();
    ASSERT_EQ(result.value(), expected);
}

TEST(Inspector, parse_inspector_ecr_finding) {
    std::string msg = "{\"version\":\"0\",\"id\":\"5b52952e-26df-3a51-6d14-4dbe737e58ec\",\"detail-type\":\"Inspector2 Finding\",\"source\":\"aws.inspector2\",\"account\":\"111122223333\",\"time\":\"2023-01-19T21:59:00Z\",\"region\":\"us-east-1\",\"resources\":[\"arn:aws:ecr:us-east-1:111122223333:repository/inspector2/sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\"],\"detail\":{\"awsAccountId\":\"111122223333\",\"description\":\"libcurlwouldreuseapreviouslycreatedconnectionevenwhenaTLSorSSHrelatedoptionhadbeenchangedthatshouldhaveprohibitedreuse.libcurlkeepspreviouslyusedconnectionsinaconnectionpoolforsubsequenttransferstoreuseifoneofthemmatchesthesetup.However,severalTLSandSSHsettingswereleftoutfromtheconfigurationmatchchecks,makingthemmatchtooeasily.\",\"exploitAvailable\":\"NO\",\"findingArn\":\"arn:aws:inspector2:us-east-1:111122223333:finding/FINDING_ID\",\"firstObservedAt\":\"Jan19,2023,9:59:00PM\",\"fixAvailable\":\"YES\",\"inspectorScore\":7.5,\"inspectorScoreDetails\":{\"adjustedCvss\":{\"adjustments\":[],\"cvssSource\":\"NVD\",\"score\":7.5,\"scoreSource\":\"NVD\",\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:H/A:N\",\"version\":\"3.1\"}},\"lastObservedAt\":\"Jan19,2023,9:59:00PM\",\"packageVulnerabilityDetails\":{\"cvss\":[{\"baseScore\":5,\"scoringVector\":\"AV:N/AC:L/Au:N/C:N/I:P/A:N\",\"source\":\"NVD\",\"version\":\"2.0\"},{\"baseScore\":7.5,\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:H/A:N\",\"source\":\"NVD\",\"version\":\"3.1\"}],\"referenceUrls\":[\"https://hackerone.com/reports/1555796\",\"https://security.gentoo.org/glsa/202212-01\",\"https://lists.debian.org/debian-lts-announce/2022/08/msg00017.html\",\"https://www.debian.org/security/2022/dsa-5197\"],\"relatedVulnerabilities\":[],\"source\":\"NVD\",\"sourceUrl\":\"https://nvd.nist.gov/vuln/detail/CVE-2022-27782\",\"vendorCreatedAt\":\"Jun2,2022,2:15:00PM\",\"vendorSeverity\":\"HIGH\",\"vendorUpdatedAt\":\"Jan5,2023,5:51:00PM\",\"vulnerabilityId\":\"CVE-2022-27782\",\"vulnerablePackages\":[{\"arch\":\"X86_64\",\"epoch\":0,\"fixedInVersion\":\"0:7.61.1-22.el8_6.3\",\"name\":\"libcurl\",\"packageManager\":\"OS\",\"release\":\"22.el8\",\"remediation\":\"yumupdatelibcurl\",\"sourceLayerHash\":\"sha256:38a980f2cc8accf69c23deae6743d42a87eb34a54f02396f3fcfd7c2d06e2c5b\",\"version\":\"7.61.1\"},{\"arch\":\"X86_64\",\"epoch\":0,\"fixedInVersion\":\"0:7.61.1-22.el8_6.3\",\"name\":\"curl\",\"packageManager\":\"OS\",\"release\":\"22.el8\",\"remediation\":\"yumupdatecurl\",\"sourceLayerHash\":\"sha256:38a980f2cc8accf69c23deae6743d42a87eb34a54f02396f3fcfd7c2d06e2c5b\",\"version\":\"7.61.1\"}]},\"remediation\":{\"recommendation\":{\"text\":\"NoneProvided\"}},\"resources\":[{\"details\":{\"awsEcrContainerImage\":{\"architecture\":\"amd64\",\"imageHash\":\"sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\",\"imageTags\":[\"o3\"],\"platform\":\"ORACLE_LINUX_8\",\"pushedAt\":\"Jan19,2023,7:38:39PM\",\"registry\":\"111122223333\",\"repositoryName\":\"inspector2\"}},\"id\":\"arn:aws:ecr:us-east-1:111122223333:repository/inspector2/sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\",\"partition\":\"aws\",\"region\":\"us-east-1\",\"type\":\"AWS_ECR_CONTAINER_IMAGE\"}],\"severity\":\"HIGH\",\"status\":\"ACTIVE\",\"title\":\"CVE-2022-27782-libcurl,curl\",\"type\":\"PACKAGE_VULNERABILITY\",\"updatedAt\":\"Jan19,2023,9:59:00PM\"}}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Inspector inspector{false, 0.0, "slack_channel", std::optional<std::string>{}};
    inspector.parse(jsonMessage);
    auto result = inspector.message();
    ASSERT_THAT(result.value(), testing::HasSubstr("Inspector2 Finding"));
    ASSERT_THAT(result.value(), testing::HasSubstr("ECR image"));
}

TEST(Inspector, parse_inspector_lambda_finding) {
    std::string msg = "{\"version\":\"0\",\"id\":\"040bb590-3a12-353f-ecb1-05e54b0fbea7\",\"detail-type\":\"Inspector2 Finding\",\"source\":\"aws.inspector2\",\"account\":\"111122223333\",\"time\":\"2023-01-19T19:20:25Z\",\"region\":\"us-east-1\",\"resources\":[\"arn:aws:lambda:us-east-1:111122223333:function:ExampleFunction:$LATEST\"],\"detail\":{\"awsAccountId\":\"111122223333\",\"description\":\"ThoseusingWoodstoxtoparseXMLdatamaybevulnerabletoDenialofServiceattacks(DOS)ifDTDsupportisenabled.Iftheparserisrunningonusersuppliedinput,anattackermaysupplycontentthatcausestheparsertocrashbystackoverflow.Thiseffectmaysupportadenialofserviceattack.\",\"exploitAvailable\":\"NO\",\"findingArn\":\"arn:aws:inspector2:us-east-1:111122223333:finding/FINDING_ID\",\"firstObservedAt\":\"Jan19,2023,7:20:25PM\",\"fixAvailable\":\"YES\",\"inspectorScore\":7.5,\"inspectorScoreDetails\":{\"adjustedCvss\":{\"cvssSource\":\"NVD\",\"score\":7.5,\"scoreSource\":\"NVD\",\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H\",\"version\":\"3.1\"}},\"lastObservedAt\":\"Jan19,2023,7:20:25PM\",\"packageVulnerabilityDetails\":{\"cvss\":[{\"baseScore\":7.5,\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H\",\"source\":\"NVD\",\"version\":\"3.1\"}],\"referenceUrls\":[\"https://bugs.chromium.org/p/oss-fuzz/issues/detail?id=47434\"],\"relatedVulnerabilities\":[],\"source\":\"NVD\",\"sourceUrl\":\"https://nvd.nist.gov/vuln/detail/CVE-2022-40152\",\"vendorCreatedAt\":\"Sep16,2022,10:15:00AM\",\"vendorSeverity\":\"HIGH\",\"vendorUpdatedAt\":\"Nov25,2022,11:15:00AM\",\"vulnerabilityId\":\"CVE-2022-40152\",\"vulnerablePackages\":[{\"epoch\":0,\"filePath\":\"lib/woodstox-core-6.2.7.jar\",\"fixedInVersion\":\"6.4.0\",\"name\":\"com.fasterxml.woodstox:woodstox-core\",\"packageManager\":\"JAR\",\"remediation\":\"Updatewoodstox-coreto6.4.0\",\"version\":\"6.2.7\"}]},\"remediation\":{\"recommendation\":{\"text\":\"NoneProvided\"}},\"resources\":[{\"details\":{\"awsLambdaFunction\":{\"architectures\":[\"X86_64\"],\"codeSha256\":\"+EwrOrht2um4fdVCD73gj+O7HJIAUvUxi8AD0eKHSkc=\",\"executionRoleArn\":\"arn:aws:iam::111122223333:role/ExampleFunction-ExecutionRole\",\"functionName\":\"Example-function\",\"lastModifiedAt\":\"Nov7,2022,8:29:27PM\",\"packageType\":\"ZIP\",\"runtime\":\"JAVA_11\",\"version\":\"$LATEST\"}},\"id\":\"arn:aws:lambda:us-east-1:111122223333:function:ExampleFunction:$LATEST\",\"partition\":\"aws\",\"region\":\"us-east-1\",\"tags\":{\"TargetAlias\":\"DeploymentStack\",\"SoftwareType\":\"Infrastructure\"},\"type\":\"AWS_LAMBDA_FUNCTION\"}],\"severity\":\"HIGH\",\"status\":\"ACTIVE\",\"title\":\"CVE-2022-40152-com.fasterxml.woodstox:woodstox-core\",\"type\":\"PACKAGE_VULNERABILITY\",\"updatedAt\":\"Jan19,2023,7:20:25PM\"}}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Inspector inspector{false, 0.0, "slack_channel", std::optional<std::string>{}};
    inspector.parse(jsonMessage);
    auto result = inspector.message();
    ASSERT_THAT(result.value(), testing::HasSubstr("Inspector2 Finding"));
    ASSERT_THAT(result.value(), testing::HasSubstr("lambda function"));
}

TEST(Inspector, if_inspector_score_lower_than_severity_threshold_return_empty) {
    std::string msg = "{\"version\":\"0\",\"id\":\"040bb590-3a12-353f-ecb1-05e54b0fbea7\",\"detail-type\":\"Inspector2 Finding\",\"source\":\"aws.inspector2\",\"account\":\"111122223333\",\"time\":\"2023-01-19T19:20:25Z\",\"region\":\"us-east-1\",\"resources\":[\"arn:aws:lambda:us-east-1:111122223333:function:ExampleFunction:$LATEST\"],\"detail\":{\"awsAccountId\":\"111122223333\",\"description\":\"ThoseusingWoodstoxtoparseXMLdatamaybevulnerabletoDenialofServiceattacks(DOS)ifDTDsupportisenabled.Iftheparserisrunningonusersuppliedinput,anattackermaysupplycontentthatcausestheparsertocrashbystackoverflow.Thiseffectmaysupportadenialofserviceattack.\",\"exploitAvailable\":\"NO\",\"findingArn\":\"arn:aws:inspector2:us-east-1:111122223333:finding/FINDING_ID\",\"firstObservedAt\":\"Jan19,2023,7:20:25PM\",\"fixAvailable\":\"YES\",\"inspectorScore\":7.5,\"inspectorScoreDetails\":{\"adjustedCvss\":{\"cvssSource\":\"NVD\",\"score\":7.5,\"scoreSource\":\"NVD\",\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H\",\"version\":\"3.1\"}},\"lastObservedAt\":\"Jan19,2023,7:20:25PM\",\"packageVulnerabilityDetails\":{\"cvss\":[{\"baseScore\":7.5,\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H\",\"source\":\"NVD\",\"version\":\"3.1\"}],\"referenceUrls\":[\"https://bugs.chromium.org/p/oss-fuzz/issues/detail?id=47434\"],\"relatedVulnerabilities\":[],\"source\":\"NVD\",\"sourceUrl\":\"https://nvd.nist.gov/vuln/detail/CVE-2022-40152\",\"vendorCreatedAt\":\"Sep16,2022,10:15:00AM\",\"vendorSeverity\":\"HIGH\",\"vendorUpdatedAt\":\"Nov25,2022,11:15:00AM\",\"vulnerabilityId\":\"CVE-2022-40152\",\"vulnerablePackages\":[{\"epoch\":0,\"filePath\":\"lib/woodstox-core-6.2.7.jar\",\"fixedInVersion\":\"6.4.0\",\"name\":\"com.fasterxml.woodstox:woodstox-core\",\"packageManager\":\"JAR\",\"remediation\":\"Updatewoodstox-coreto6.4.0\",\"version\":\"6.2.7\"}]},\"remediation\":{\"recommendation\":{\"text\":\"NoneProvided\"}},\"resources\":[{\"details\":{\"awsLambdaFunction\":{\"architectures\":[\"X86_64\"],\"codeSha256\":\"+EwrOrht2um4fdVCD73gj+O7HJIAUvUxi8AD0eKHSkc=\",\"executionRoleArn\":\"arn:aws:iam::111122223333:role/ExampleFunction-ExecutionRole\",\"functionName\":\"Example-function\",\"lastModifiedAt\":\"Nov7,2022,8:29:27PM\",\"packageType\":\"ZIP\",\"runtime\":\"JAVA_11\",\"version\":\"$LATEST\"}},\"id\":\"arn:aws:lambda:us-east-1:111122223333:function:ExampleFunction:$LATEST\",\"partition\":\"aws\",\"region\":\"us-east-1\",\"tags\":{\"TargetAlias\":\"DeploymentStack\",\"SoftwareType\":\"Infrastructure\"},\"type\":\"AWS_LAMBDA_FUNCTION\"}],\"severity\":\"HIGH\",\"status\":\"ACTIVE\",\"title\":\"CVE-2022-40152-com.fasterxml.woodstox:woodstox-core\",\"type\":\"PACKAGE_VULNERABILITY\",\"updatedAt\":\"Jan19,2023,7:20:25PM\"}}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Inspector inspector{false, 10.0, "slack_channel", std::optional<std::string>{}};
    inspector.parse(jsonMessage);
    auto result = inspector.message();
    ASSERT_THAT(result.has_value(), false);
}

TEST(Inspector, if_latest_enabled_should_not_return_ECR_findings_with_no_tag_latest) {
    std::string msg = "{\"version\":\"0\",\"id\":\"5b52952e-26df-3a51-6d14-4dbe737e58ec\",\"detail-type\":\"Inspector2 Finding\",\"source\":\"aws.inspector2\",\"account\":\"111122223333\",\"time\":\"2023-01-19T21:59:00Z\",\"region\":\"us-east-1\",\"resources\":[\"arn:aws:ecr:us-east-1:111122223333:repository/inspector2/sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\"],\"detail\":{\"awsAccountId\":\"111122223333\",\"description\":\"libcurlwouldreuseapreviouslycreatedconnectionevenwhenaTLSorSSHrelatedoptionhadbeenchangedthatshouldhaveprohibitedreuse.libcurlkeepspreviouslyusedconnectionsinaconnectionpoolforsubsequenttransferstoreuseifoneofthemmatchesthesetup.However,severalTLSandSSHsettingswereleftoutfromtheconfigurationmatchchecks,makingthemmatchtooeasily.\",\"exploitAvailable\":\"NO\",\"findingArn\":\"arn:aws:inspector2:us-east-1:111122223333:finding/FINDING_ID\",\"firstObservedAt\":\"Jan19,2023,9:59:00PM\",\"fixAvailable\":\"YES\",\"inspectorScore\":7.5,\"inspectorScoreDetails\":{\"adjustedCvss\":{\"adjustments\":[],\"cvssSource\":\"NVD\",\"score\":7.5,\"scoreSource\":\"NVD\",\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:H/A:N\",\"version\":\"3.1\"}},\"lastObservedAt\":\"Jan19,2023,9:59:00PM\",\"packageVulnerabilityDetails\":{\"cvss\":[{\"baseScore\":5,\"scoringVector\":\"AV:N/AC:L/Au:N/C:N/I:P/A:N\",\"source\":\"NVD\",\"version\":\"2.0\"},{\"baseScore\":7.5,\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:H/A:N\",\"source\":\"NVD\",\"version\":\"3.1\"}],\"referenceUrls\":[\"https://hackerone.com/reports/1555796\",\"https://security.gentoo.org/glsa/202212-01\",\"https://lists.debian.org/debian-lts-announce/2022/08/msg00017.html\",\"https://www.debian.org/security/2022/dsa-5197\"],\"relatedVulnerabilities\":[],\"source\":\"NVD\",\"sourceUrl\":\"https://nvd.nist.gov/vuln/detail/CVE-2022-27782\",\"vendorCreatedAt\":\"Jun2,2022,2:15:00PM\",\"vendorSeverity\":\"HIGH\",\"vendorUpdatedAt\":\"Jan5,2023,5:51:00PM\",\"vulnerabilityId\":\"CVE-2022-27782\",\"vulnerablePackages\":[{\"arch\":\"X86_64\",\"epoch\":0,\"fixedInVersion\":\"0:7.61.1-22.el8_6.3\",\"name\":\"libcurl\",\"packageManager\":\"OS\",\"release\":\"22.el8\",\"remediation\":\"yumupdatelibcurl\",\"sourceLayerHash\":\"sha256:38a980f2cc8accf69c23deae6743d42a87eb34a54f02396f3fcfd7c2d06e2c5b\",\"version\":\"7.61.1\"},{\"arch\":\"X86_64\",\"epoch\":0,\"fixedInVersion\":\"0:7.61.1-22.el8_6.3\",\"name\":\"curl\",\"packageManager\":\"OS\",\"release\":\"22.el8\",\"remediation\":\"yumupdatecurl\",\"sourceLayerHash\":\"sha256:38a980f2cc8accf69c23deae6743d42a87eb34a54f02396f3fcfd7c2d06e2c5b\",\"version\":\"7.61.1\"}]},\"remediation\":{\"recommendation\":{\"text\":\"NoneProvided\"}},\"resources\":[{\"details\":{\"awsEcrContainerImage\":{\"architecture\":\"amd64\",\"imageHash\":\"sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\",\"imageTags\":[\"o3\"],\"platform\":\"ORACLE_LINUX_8\",\"pushedAt\":\"Jan19,2023,7:38:39PM\",\"registry\":\"111122223333\",\"repositoryName\":\"inspector2\"}},\"id\":\"arn:aws:ecr:us-east-1:111122223333:repository/inspector2/sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\",\"partition\":\"aws\",\"region\":\"us-east-1\",\"type\":\"AWS_ECR_CONTAINER_IMAGE\"}],\"severity\":\"HIGH\",\"status\":\"ACTIVE\",\"title\":\"CVE-2022-27782-libcurl,curl\",\"type\":\"PACKAGE_VULNERABILITY\",\"updatedAt\":\"Jan19,2023,9:59:00PM\"}}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Inspector inspector{true, 0.0, "slack_channel", std::optional<std::string>{}};
    inspector.parse(jsonMessage);
    auto result = inspector.message();
    ASSERT_THAT(result.has_value(), false);
}

TEST(Inspector, ECR_tag_is_latest_and_onlylatest_is_enabled) {
    std::string msg = "{\"version\":\"0\",\"id\":\"5b52952e-26df-3a51-6d14-4dbe737e58ec\",\"detail-type\":\"Inspector2 Finding\",\"source\":\"aws.inspector2\",\"account\":\"111122223333\",\"time\":\"2023-01-19T21:59:00Z\",\"region\":\"us-east-1\",\"resources\":[\"arn:aws:ecr:us-east-1:111122223333:repository/inspector2/sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\"],\"detail\":{\"awsAccountId\":\"111122223333\",\"description\":\"libcurlwouldreuseapreviouslycreatedconnectionevenwhenaTLSorSSHrelatedoptionhadbeenchangedthatshouldhaveprohibitedreuse.libcurlkeepspreviouslyusedconnectionsinaconnectionpoolforsubsequenttransferstoreuseifoneofthemmatchesthesetup.However,severalTLSandSSHsettingswereleftoutfromtheconfigurationmatchchecks,makingthemmatchtooeasily.\",\"exploitAvailable\":\"NO\",\"findingArn\":\"arn:aws:inspector2:us-east-1:111122223333:finding/FINDING_ID\",\"firstObservedAt\":\"Jan19,2023,9:59:00PM\",\"fixAvailable\":\"YES\",\"inspectorScore\":7.5,\"inspectorScoreDetails\":{\"adjustedCvss\":{\"adjustments\":[],\"cvssSource\":\"NVD\",\"score\":7.5,\"scoreSource\":\"NVD\",\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:H/A:N\",\"version\":\"3.1\"}},\"lastObservedAt\":\"Jan19,2023,9:59:00PM\",\"packageVulnerabilityDetails\":{\"cvss\":[{\"baseScore\":5,\"scoringVector\":\"AV:N/AC:L/Au:N/C:N/I:P/A:N\",\"source\":\"NVD\",\"version\":\"2.0\"},{\"baseScore\":7.5,\"scoringVector\":\"CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:H/A:N\",\"source\":\"NVD\",\"version\":\"3.1\"}],\"referenceUrls\":[\"https://hackerone.com/reports/1555796\",\"https://security.gentoo.org/glsa/202212-01\",\"https://lists.debian.org/debian-lts-announce/2022/08/msg00017.html\",\"https://www.debian.org/security/2022/dsa-5197\"],\"relatedVulnerabilities\":[],\"source\":\"NVD\",\"sourceUrl\":\"https://nvd.nist.gov/vuln/detail/CVE-2022-27782\",\"vendorCreatedAt\":\"Jun2,2022,2:15:00PM\",\"vendorSeverity\":\"HIGH\",\"vendorUpdatedAt\":\"Jan5,2023,5:51:00PM\",\"vulnerabilityId\":\"CVE-2022-27782\",\"vulnerablePackages\":[{\"arch\":\"X86_64\",\"epoch\":0,\"fixedInVersion\":\"0:7.61.1-22.el8_6.3\",\"name\":\"libcurl\",\"packageManager\":\"OS\",\"release\":\"22.el8\",\"remediation\":\"yumupdatelibcurl\",\"sourceLayerHash\":\"sha256:38a980f2cc8accf69c23deae6743d42a87eb34a54f02396f3fcfd7c2d06e2c5b\",\"version\":\"7.61.1\"},{\"arch\":\"X86_64\",\"epoch\":0,\"fixedInVersion\":\"0:7.61.1-22.el8_6.3\",\"name\":\"curl\",\"packageManager\":\"OS\",\"release\":\"22.el8\",\"remediation\":\"yumupdatecurl\",\"sourceLayerHash\":\"sha256:38a980f2cc8accf69c23deae6743d42a87eb34a54f02396f3fcfd7c2d06e2c5b\",\"version\":\"7.61.1\"}]},\"remediation\":{\"recommendation\":{\"text\":\"NoneProvided\"}},\"resources\":[{\"details\":{\"awsEcrContainerImage\":{\"architecture\":\"amd64\",\"imageHash\":\"sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\",\"imageTags\":[\"latest\"],\"platform\":\"ORACLE_LINUX_8\",\"pushedAt\":\"Jan19,2023,7:38:39PM\",\"registry\":\"111122223333\",\"repositoryName\":\"inspector2\"}},\"id\":\"arn:aws:ecr:us-east-1:111122223333:repository/inspector2/sha256:98f0304b3a3b7c12ce641177a99d1f3be56f532473a528fda38d53d519cafb13\",\"partition\":\"aws\",\"region\":\"us-east-1\",\"type\":\"AWS_ECR_CONTAINER_IMAGE\"}],\"severity\":\"HIGH\",\"status\":\"ACTIVE\",\"title\":\"CVE-2022-27782-libcurl,curl\",\"type\":\"PACKAGE_VULNERABILITY\",\"updatedAt\":\"Jan19,2023,9:59:00PM\"}}";
    nlohmann::json json;
    auto jsonMessage = json.parse(msg);
    Inspector inspector{true, 0.0, "slack_channel", std::optional<std::string>{}};
    inspector.parse(jsonMessage);
    auto result = inspector.message();
    ASSERT_THAT(result.has_value(), true);
}