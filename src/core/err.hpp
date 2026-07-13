#pragma once

#include "i18n.hpp"
#include <QString>
#include <backward.hpp>
#include <cstdint>

constexpr const char *errmsgs[] = {
    QT_TRANSLATE_NOOP("errmsgs", "Extern Error"),
    QT_TRANSLATE_NOOP("errmsgs", "Empty ID"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid ID"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid unicode colored character"),
    QT_TRANSLATE_NOOP("errmsgs", "No such element in this kit"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid location in this large-block"),
    QT_TRANSLATE_NOOP("errmsgs", "Failed to parse JSON"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid email address"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid RGB color"),
    QT_TRANSLATE_NOOP("errmsgs", "Author doesn't exist"),
    QT_TRANSLATE_NOOP("errmsgs", "Map data doesn't exist"),
    QT_TRANSLATE_NOOP("errmsgs", "No such kit in all loaded kits"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid Element ID; it should be like \"kit/elem\""),
    QT_TRANSLATE_NOOP("errmsgs", "Product version doesn't exist"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid location in this map")};

// Craftive Exceptions
class CrtExcept {
private:
    const uint16_t             code; // Error Code (0x0000 ~ 0xFFFF)
    const QString              detail;
    const backward::StackTrace st;

public:
    CrtExcept(const uint16_t _code, const QString &_detail, auto &&...args);

    auto how() const noexcept -> const QString &;                     // Return detail
    auto stacktrace() const noexcept -> const backward::StackTrace &; // Return st
    auto what() const noexcept -> QString;   // Return errmsgs[code] (translated)
    auto which() const noexcept -> uint16_t; // Return code
    auto whichStr() const -> QString;        // Return code as QString
};
