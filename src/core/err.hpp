#pragma once

#include "backward.hpp"
#include <QString>
#include <cstdint>

// Craftive Exceptions
class CrtExcept {
private:
    const uint16_t       code; // Error Code (0x0000 ~ 0xFFFF)
    const QString        detail;
    backward::StackTrace st;

public:
    CrtExcept(const uint16_t _code, const QString &_detail, auto &&...args);

    auto how() const noexcept -> const QString &;                     // Return detail
    auto stacktrace() const noexcept -> const backward::StackTrace &; // Return st
    auto what() const noexcept -> QString;   // Return errmsgs[code] (translated)
    auto which() const noexcept -> uint16_t; // Return code
    auto whichStr() const -> QString;        // Return code as QString
};
