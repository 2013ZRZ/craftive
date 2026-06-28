#pragma once

#include "crtutils.hpp"
#include "localize.hpp"
#include <cstdint>

constexpr const char *errmsgs[] = {"Extern Error",
                                   "Empty ID",
                                   "Invalid ID",
                                   "Invalid unicode colored character",
                                   "No such element in this kit",
                                   "Invalid location in this large-block",
                                   "Failed to parse JSON",
                                   "Invalid email address",
                                   "Invalid RGB color",
                                   "Empty author",
                                   "Empty map data",
                                   "No such element in all loaded kits",
                                   "Invalid Element ID; it should be like \"kit/elem\"",
                                   "Empty product version"};

// Craftive Exceptions
class CrtExcept : public std::exception {
  private:
    const uint16_t code; // Error Code (0x0000 ~ 0xFFFF)
    const QString  detail;

  public:
    CrtExcept(const uint16_t _code, const isStr auto &_detail, auto &&...args)
        : code(_code), detail(std::vformat(_detail, std::make_format_args(args...))) {}

    CrtExcept(const isStr auto &_detail, auto &&...args) : CrtExcept(0x0000, _detail, args...) {}

    const QString &how() const noexcept;           // Return the value of detail
    const char    *what() const noexcept override; // Return the value of errmsgs[code]
    const uint16_t which() const noexcept;         // Return the value of code
    const QString  whichStr() const;               // Return the value of code as QString
};
