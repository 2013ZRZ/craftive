#include "err.hpp"
#include <format>

const QString &CrtExcept::how() const noexcept { return detail; }

const char *CrtExcept::what() const noexcept { return _(errmsgs[code]); }

const uint16_t CrtExcept::which() const noexcept { return code; }

const QString CrtExcept::whichStr() const {
    return QString::fromStdString(std::format("0x{:04X}", code));
}
