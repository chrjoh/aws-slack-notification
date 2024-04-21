#pragma once
#include <algorithm>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>

template <typename T>
class Var {
   private:
    T value;
    virtual std::optional<std::string> getEnvVar(std::string const &key) const;
    void setValue(std::optional<std::string> &var);

   public:
    Var(std::string const &name);
    Var(std::string const &name, T v);
    T get() const;

    virtual ~Var() = default;
};
template <typename T>
Var<T>::Var(std::string const &name, T v) {
    auto var = getEnvVar(name);
    if (var.has_value()) {
        setValue(var);
    } else {
        value = v;
    }
}

template <typename T>
Var<T>::Var(std::string const &name) {
    auto var = getEnvVar(name);
    if (var.has_value()) {
        setValue(var);
    } else {
        throw(std::domain_error("missing mandatory environmen variable: " + name));
    }
}

template <typename T>
void Var<T>::setValue(std::optional<std::string> &var) {
    if constexpr (std::is_same<T, int>::value) {
        value = std::stoi(var.value());
    } else if constexpr (std::is_same<T, bool>::value) {
        bool tmp;
        std::string str = var.value();
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        std::istringstream is(str);
        is >> std::boolalpha >> tmp;
        value = tmp;
    } else if constexpr (std::is_same<T, double>::value) {
        value = std::stod(var.value());
    } else {
        value = var.value();
    }
}

template <typename T>
T Var<T>::get() const {
    return value;
}

template <typename T>
std::optional<std::string> Var<T>::getEnvVar(std::string const &key) const {
    char const *val = getenv(key.c_str());
    if (val == NULL) {
        return {};
    } else {
        return std::string(val);
    }
}
