#pragma once

#include <exception>
#include <libintl.h>
#include <string>

#define UNKNOWNERR "0xFFFF Unknown error"
#define _(str) gettext(str)

bool        isValidID(const std::string &id);
std::string randomID();

constexpr std::string_view errmsgs[] = {
    "Empty ID",
    "The ID includes invalid character(s)",
    "Invalid color code; type `help --color-code` for more info",
    "No such element in this kit",
    "Invalid position",
    "Failed to parse JSON"};

// Craftive Exceptions
class CrtExcept : public std::exception {
  private:
    unsigned short      code;
    std::string         msg;
    mutable std::string _what; // Where stores the result of what().

  public:
    CrtExcept(const unsigned short _code);
    CrtExcept(const std::string &_msg);
    const char    *what() const noexcept;
    unsigned short which() const noexcept;
};