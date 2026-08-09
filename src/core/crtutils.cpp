#include "crtutils.hpp"
#include "err.hpp"
#include "qcoreapplication.h"
#include <QRegularExpression>
#include <ctime>
#include <random>

Version::Version(uint8_t ma, uint8_t mi, uint8_t p) : major(ma), minor(mi), patch(p) {}

auto Version::operator<=>(const Version &other) const noexcept -> std::strong_ordering {
    if (major != other.major)
        return major <=> other.major;
    else if (minor != other.minor)
        return minor <=> other.minor;
    else
        return patch <=> other.patch;
}

bool Version::operator==(const Version &other) const noexcept {
    return major == other.major && minor == other.minor && patch == other.patch;
}

QString Version::operator()() const {
    try {
        return QString{"%1.%2.%3"}.arg(major).arg(minor).arg(patch);
    }
    CATCH_THROW(Version::operator())
}

void Version::fromJson(const json &j) {
    try {
        // j should be an array like [x, y, z] for "x.y.z"
        if (!j.is_array())
            throw CrtExcept(0x0006,
                            tr("From Version::fromJson(); the version's JSON isn't an array. It "
                               "should be like [x, y, z] for \"x.y.z\""));
        major = j[0];
        minor = j[1];
        patch = j[2];
    }
    CATCH_THROW(Version::fromJson())
}

json Version::toJson() const {
    try {
        return json{major, minor, patch};
    }
    CATCH_THROW(Version::toJson())
}

template <typename T>
    requires std::is_nothrow_default_constructible_v<T> && std::is_move_constructible_v<T>
auto OptionalOnStack<T>::operator=(T &&other) noexcept -> OptionalOnStack<T> & {
    value = other;
    has   = true;
    return *this;
}

template <typename T>
    requires std::is_nothrow_default_constructible_v<T> && std::is_move_constructible_v<T>
auto OptionalOnStack<T>::operator=(OptionalOnStack<T> &&other) noexcept -> OptionalOnStack<T> & {
    if (other.has) {
        value = other.value;
        has   = true;
    }
    return *this;
}

template <typename T>
    requires std::is_nothrow_default_constructible_v<T> && std::is_move_constructible_v<T>
OptionalOnStack<T>::operator bool() const noexcept {
    return has;
}

template <typename T>
    requires std::is_nothrow_default_constructible_v<T> && std::is_move_constructible_v<T>
void OptionalOnStack<T>::reset() noexcept {
    value = {};
    has   = false;
}

void checkID(const QString &id) {
    try {
        if (id.isEmpty())
            throw CrtExcept(0x0001, QCoreApplication::translate("checkID", "From checkID()"));
        for (const QChar &ch : id) {
            if (!ch.isLetterOrNumber() && ch != u'_')
                throw CrtExcept(0x0002, QCoreApplication::translate("checkID", "From checkID()"));
        }
    }
    CATCH_THROW(checkID())
}

void checkElemID(const QString &id) {
    try {
        checkID(id);
        if (id.split(u'/').size() != 2)
            throw CrtExcept(
                0x0002,
                QCoreApplication::translate("checkElemID",
                                            "From checkElemID(); the ID isn't like \"kit/elem\""));
    }
    CATCH_THROW(checkElemID())
}

namespace EmailIdentifierDetail {
static const QRegularExpression emailre{
    "\\A\\w+([-+.']\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*\\z"};
}

void checkEmail(const QString &email) {
    try {
        if (!EmailIdentifierDetail::emailre.match(email).hasMatch())
            throw CrtExcept(0x0007,
                            QCoreApplication::translate(
                                "checkEmail",
                                "From checkEmail(); the string is \"%1\" and it isn't a valid "
                                "email address"),
                            email);
    }
    CATCH_THROW(checkEmail())
}

QString randomID(uint8_t len) {
    try {
        std::random_device rd;
        QString            o;
        std::mt19937_64    r(time(nullptr) + rd());
        for (uint8_t i{}; i < len; i++) {
            uint8_t _r = r() % 63;
            if (_r < 26)
                o += (QChar)('a' + _r); // 0 ~ 25 : 'a' ~ 'z'
            else if (_r < 52)
                o += (QChar)('A' + _r - 26); // 26 ~ 51 : 'A' ~ 'Z'
            else if (_r < 62)
                o += (QChar)('0' + _r - 52); // 52 ~ 61 : '0' ~ '9'
            else
                o += '_'; // 62 : '_'
        }
        return o;
    }
    CATCH_THROW(randomID())
}

auto separateElemID(const QString &s) -> IDSeparatorDetail::SeparatedElemID {
    try {
        auto list = s.split(QChar{u'/'});
        if (list.size() != 2)
            throw CrtExcept(0x000C,
                            QCoreApplication::translate("separateElemID",
                                                        "From separateElemID(); the ID is %1"),
                            s);
        return IDSeparatorDetail::SeparatedElemID{.kit = list[0], .elem = list[1]};
    }
    CATCH_THROW(separateElemID)
}