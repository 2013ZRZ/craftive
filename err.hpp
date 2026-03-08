#pragma once

#include "localize.hpp"
#include <concepts>
#include <cstdint>
#include <format>
#include <string>
#include <string_view>

bool        isInvalidID(const std::string &id);
std::string randomID(unsigned len = 50);

constexpr const char *errmsgs[] = {"Extern Error",
                                   "Empty ID",
                                   "The ID includes invalid character(s)",
                                   "Invalid color code",
                                   "No such element in this kit",
                                   "Invalid location in this large-block",
                                   "Failed to parse JSON"};

// Craftive Exceptions
class CrtExcept : public std::exception {
  private:
    uint16_t          code; // Error Code (0x0000 ~ 0xFFFF)
    const std::string detail;

  public:
    CrtExcept(const uint16_t _code, const std::string &_detail, auto &&...args)
        : code(_code), detail(std::vformat(_detail, std::make_format_args(args...))) {}

    template <typename T>
    CrtExcept(T &&_detail, auto &&...args)
        requires(std::same_as<std::decay_t<T>, char *> ||
                 std::same_as<std::decay_t<T>, std::string> ||
                 std::same_as<std::decay_t<T>, std::string_view>)
        : CrtExcept(0x0000, _detail, args...) {}

    const std::string &how() const noexcept;   // Return the value of detail
    const char        *what() const noexcept;  // Return the value of errmsgs[code]
    uint16_t           which() const noexcept; // Return the value of code
    const std::string  whichStr() const;       // Return the std::string version of code
};
