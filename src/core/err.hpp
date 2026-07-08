#pragma once

#include "i18n.hpp"
#include <QtCore/QString>
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
    const uint16_t code; // Error Code (0x0000 ~ 0xFFFF)
    const QString  detail;

  public:
    CrtExcept(const uint16_t _code, const QString &_detail, auto &&...args);

    const QString &how() const noexcept;   // Return the value of detail
    const QString  what() const noexcept;  // Return the value of errmsgs[code]
    const uint16_t which() const noexcept; // Return the value of code
    const QString  whichStr() const;       // Return the value of code as QString
};
