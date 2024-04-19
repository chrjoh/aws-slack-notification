#!/bin/bash
cmake -B build -S .
cmake --build build --target aws-lambda-package-aws-slack-notification