#pragma once

#include "crtutils.hpp"
#include "localize.hpp"
#include <cstdint>

constexpr const char *errmsgs[] = {"Extern Error",
                                   "Empty ID",
                                   "The ID includes invalid character(s)",
                                   "Invalid unicode colored character",
                                   "No such element in this kit",
                                   "Invalid location in this large-block",
                                   "Failed to parse JSON",
                                   "Invalid large-block; it should be a rectangle",
                                   "Invalid RGB color"};

// Craftive Exceptions
class CrtExcept : public std::exception {
  private:
    const uint16_t    code; // Error Code (0x0000 ~ 0xFFFF)
    const std::string detail;

  public:
    CrtExcept(const uint16_t _code, const is_string auto &_detail, auto &&...args)
        : code(_code), detail(std::vformat(_detail, std::make_format_args(args...))) {}

    CrtExcept(const is_string auto &_detail, auto &&...args)
        : CrtExcept(0x0000, _detail, args...) {}

    const std::string &how() const noexcept;           // Return the value of detail
    const char        *what() const noexcept override; // Return the value of errmsgs[code]
    const uint16_t     which() const noexcept;         // Return the value of code
    const std::string  whichStr() const;               // Return the value of code as std::string
};
