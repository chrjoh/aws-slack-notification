#!/bin/bash
docker build --rm -t slack-notification .
mkdir -p release_build
docker run --platform linux/arm64 -v $PWD/release_build:/release_build  -t  slack-notification
