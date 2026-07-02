#include "err.hpp"

const QString &CrtExcept::how() const noexcept { return detail; }

const QString CrtExcept::what() const noexcept { return QCoreApplication::tr(errmsgs[code]); }

const uint16_t CrtExcept::which() const noexcept { return code; }

const QString CrtExcept::whichStr() const { return QString{"0x%1"}.arg(code, 4, 16, QChar{'0'}); }
