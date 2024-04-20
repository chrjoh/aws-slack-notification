# AWS Slack notification

Lambda that takes messages from AWS SNS or event bridge and publish them to a slack channel.
Currently supports:

- GuardDuty from event bridge
- AWS health from event bridge
- Inspector2 alerts from event bridge
- Cloudwatch Alarm from SNS 

### Usage
- create a lambda funcrion on AWS that use the layer as specified at https://docs.aws.amazon.com/systems-manager/latest/userguide/ps-integration-lambda-extensions.html

- Create a slack app to recieve a OAth token
- Store the OAuth token in systems manager parameter store
- The follwoing enviroment variables for the lambda are mandatory:
  - OAUTH_TOKEN_KEY, string that is the key in SSM parameter store for the slack oauth token
  - SLACK_CHANNEL, if the channel is private make sure to add tha slack app to that channel.
- Optional environments:
  - THRESHOLD, double with the minimum score inspector2 must have for a message to be sent default to 0.0
  - ONLYLATEST, bool if only images in ECR`with tag latest should be reported to slack for inspector, default to false.
  - AWS_ACCOUNT_NAME(not implemented yet), string friendly name that will be added to slack message if not present accoint id is used.
  - AWS_HEALTH_EVENTS_TO_SKIP comma separated list of AWS health, using the value of `eventTypeCode` events to ignore

- Docker is used for building the release, the release is built for arm architecture.
- Build the function by running `./release_build.sh` the resulting lambda function will created as zip file in the folder `release_build`
- Upload the zip file to your AWS lambda function 

## Development
Make sure you have the following applications available on your system.
- cmake (https://cmake.org/)
- vcpkg (https://vcpkg.io/en/)
- vscode (https://code.visualstudio.com/)

The project contains all the configurations to be used with vscode and CMakePresets for easy setup of build/debug targets.