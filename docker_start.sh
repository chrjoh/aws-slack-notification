#!/bin/bash
docker run --platform linux/arm64  -t -i slack-notification /bin/bash

# get relase build
#docker run --platform linux/arm64 -v $HOME/projects/tv4/slack-notification/release_build:/release_build  -t  slack-notification