#include "client.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

// auto params = UnorderedElementsAre(Pair("WithDecryption", "true"), Pair("name", "key"));
TEST(HttpClient, add_query_string_to_url) {
    HttpClient client{};
    Params params = {{"WithDecryption", "true"},
                     {"name", "key with space"}};
    auto query = client.paramsString(params);
    ASSERT_EQ(query, "?WithDecryption=true&name=key%20with%20space");
};
