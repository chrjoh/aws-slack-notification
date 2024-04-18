#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "var.hpp"

TEST(Var, create_int_with_default) {
    auto v = Var<int>("int", 10);
    ASSERT_EQ(v.get(), 10);
}
TEST(Var, create_bool_with_default) {
    auto v = Var<bool>("bool", true);
    ASSERT_EQ(v.get(), true);
}
TEST(Var, create_double_with_default) {
    auto v = Var<double>("double", 7.5);
    ASSERT_THAT(v.get(), testing::DoubleEq(7.5));
}
TEST(Var, create_string_with_default) {
    auto v = Var<std::string>("string", "10");
    ASSERT_EQ(v.get(), "10");
}

TEST(Var, create_int_from_environment) {
    setenv("int-test-var", "10", 0);
    auto v = Var<int>("int-test-var");
    ASSERT_EQ(v.get(), 10);
}
TEST(Var, create_bool_from_environment) {
    setenv("bool-test-var", "True", 0);
    auto v = Var<bool>("bool-test-var");
    ASSERT_EQ(v.get(), true);
}
TEST(Var, create_double_from_environment) {
    setenv("double-test-var", "7.5", 0);
    auto v = Var<double>("double-test-var");
    ASSERT_THAT(v.get(), testing::DoubleEq(7.5));
}
TEST(Var, create_string_from_environment) {
    setenv("string-test-var", "10", 0);
    auto v = Var<std::string>("string-test-var");
    ASSERT_EQ(v.get(), "10");
}

TEST(Var, try_to_create_non_optional_without_setting_env) {
    ASSERT_ANY_THROW(Var<int>("int1234-non-existing"));
}