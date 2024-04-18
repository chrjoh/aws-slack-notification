#!/bin/bash
cmake -B build -S .
cmake --build build --target aws-lambda-package-slack-notification