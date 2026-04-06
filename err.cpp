#include "err.hpp"
#include <cctype>
#include <format>

const std::string &CrtExcept::how() const noexcept { return detail; }

const char *CrtExcept::what() const noexcept { return _(errmsgs[code]); }

const uint16_t CrtExcept::which() const noexcept { return code; }

const std::string CrtExcept::whichStr() const { return std::format("0x{:04X}", code); }
