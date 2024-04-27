// main.cpp
#include <aws/lambda-runtime/runtime.h>

#include <iostream>
#include <string>

#include "environment.hpp"
#include "handler/handler.hpp"
#include "http/client.hpp"

using namespace aws::lambda_runtime;

invocation_response my_handler(invocation_request const& request) {
    try {
        HttpClient client;
        Environment env{std::make_shared<HttpClient>(client)};
        Handler handler(std::make_shared<HttpClient>(client), env);
        if (request.payload.size() > 0) {
            std::string payload = request.payload;
            // std::cerr << request.payload << std::endl;
            handler.process(payload);
        }
    } catch (const std::exception& ex) {
        std::cerr << "ERROR handling payload" << std::endl;
        std::cerr << "payload: " << request.payload << std::endl;
        std::cerr << "exception: " << ex.what() << std::endl;
        return invocation_response::failure("Failed to handle message", ex.what());
    }

    return invocation_response::success("SNS message parsed", "application/json");
}

int main() {
    run_handler(my_handler);
    return 0;
}
