#include "err.hpp"
#include <QCoreApplication>

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

CrtExcept::CrtExcept(const uint16_t _code, const QString &_detail, auto &&...args)
    : code(_code), detail(_detail.arg(args...)) {
    st.load_here(32);
}

auto CrtExcept::how() const noexcept -> const QString & { return detail; }

auto CrtExcept::stacktrace() const noexcept -> const backward::StackTrace & { return st; }

auto CrtExcept::what() const noexcept -> QString {
    return QCoreApplication::translate("errmsgs", errmsgs[code]);
}

auto CrtExcept::which() const noexcept -> uint16_t { return code; }

auto CrtExcept::whichStr() const -> QString { return QString{"0x%1"}.arg(code, 4, 16, QChar{'0'}); }
