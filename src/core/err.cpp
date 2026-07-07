#include "err.hpp"
#include "i18n.hpp"

CrtExcept::CrtExcept(const uint16_t _code, const QString &_detail, auto &&...args)
    : code(_code), detail(_detail.arg(args...)) {}

const QString &CrtExcept::how() const noexcept { return detail; }

const QString CrtExcept::what() const noexcept { return translate("errmsgs", errmsgs[code]); }

const uint16_t CrtExcept::which() const noexcept { return code; }

const QString CrtExcept::whichStr() const { return QString{"0x%1"}.arg(code, 4, 16, QChar{'0'}); }
