#pragma once

#include <concepts>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

bool           isInvalidID(const std::string &id);
std::string    randomID(unsigned len = 50);
std::u32string s2u32s(const std::string &s);    // string -> u32string
std::string    u32s2s(const std::u32string &s); // u32string -> string

template <typename T>
concept is_string =
    std::same_as<std::decay_t<T>, char *> || std::same_as<std::decay_t<T>, std::string> ||
    std::same_as<std::decay_t<T>, std::string_view>;

template <typename T>
concept is_u32string =
    std::same_as<std::decay_t<T>, char32_t *> || std::same_as<std::decay_t<T>, std::u32string> ||
    std::same_as<std::decay_t<T>, std::u32string_view>;

template <typename T>
concept is_json = std::same_as<std::decay_t<T>, json>;