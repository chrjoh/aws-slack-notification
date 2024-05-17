#!/bin/bash
docker build --rm -t aws-slack-notification .
mkdir -p release_build
docker run --rm --platform linux/arm64 -v $PWD/release_build:/release_build  -t  aws-slack-notification
