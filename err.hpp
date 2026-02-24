#pragma once

#include "localize.hpp"
#include <exception>
#include <string>

#define UNKNOWNERR "0xFFFF Unknown error"

bool        isInvalidID(const std::string &id);
std::string randomID(unsigned len = 50);

constexpr const char *errmsgs[] = {[0x0000] = "Extern Error",
                                   [0x0001] = "Empty ID",
                                   [0x0002] = "The ID includes invalid character(s)",
                                   [0x0003] =
                                       "Invalid color code; type `help --color-code` for more info",
                                   [0x0004] = "No such element in this kit",
                                   [0x0005] = "Invalid location in this large-block",
                                   [0x0006] = "Failed to parse JSON"};

// Craftive Exceptions
class CrtExcept : public std::exception {
  private:
    unsigned short code; // Error Code (0x0000 ~ 0xFFFF)
    const char    *detail;

  public:
    CrtExcept(const unsigned short _code, const char *_detail);
    CrtExcept(const char *_detail);
    const char    *how() const noexcept;   // Return the value of detail
    const char    *what() const noexcept;  // Return the value of errmsgs[code]
    unsigned short which() const noexcept; // Return the value of code
    std::string    whichStr() const;       // Return the std::string version of code
};
