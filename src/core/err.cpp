#include "err.hpp"

CrtExcept::CrtExcept(const uint16_t _code, const QString &_detail, auto &&...args)
    : code(_code), detail(_detail.arg(args...)) {
    st.load_here(32);
}

auto CrtExcept::how() const noexcept -> const QString & { return detail; }

auto CrtExcept::stacktrace() const noexcept -> const backward::StackTrace & { return st; }

auto CrtExcept::what() const noexcept -> QString { return translate("errmsgs", errmsgs[code]); }

auto CrtExcept::which() const noexcept -> uint16_t { return code; }

auto CrtExcept::whichStr() const -> QString { return QString{"0x%1"}.arg(code, 4, 16, QChar{'0'}); }
