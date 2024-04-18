#include "environment.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "var.hpp"

TEST(Environment, split_string_into_vector_with_no_delimeter_in_string) {
    Environment env{};
    std::string data = "key1";
    std::vector<std::string> result = env.getStrings(data, ",");
    EXPECT_EQ(result.size(), 1);
}

TEST(Environment, split_string_into_vector_removing_leading_trailing_whitespaces) {
    Environment env{};
    std::string data = "key1, key2 ,key3";
    std::vector<std::string> result = env.getStrings(data, ",");
    EXPECT_EQ(result[0], "key1");
    EXPECT_EQ(result[1], "key2");
    EXPECT_EQ(result[2], "key3");
}

TEST(Environment, split_string_into_vector_and_handle_last_char_is_delimeter) {
    Environment env{};
    std::string data = "key1, key2 ,key3,   ";
    std::vector<std::string> result = env.getStrings(data, ",");
    EXPECT_EQ(result.size(), 3);
}
