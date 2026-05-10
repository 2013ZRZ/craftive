#include "crtutils.hpp"
#include <ctime>
#include <random>
#include <regex>

bool isInvalidID(const QStringView &id) {
    for (const auto it : id.toString().toStdString()) {
        if (!isalnum(it) && it != '_')
            return true;
    }
    return false;
}

namespace EmailIdentifierDetail {
static const std::regex emailre("^\\w+([-+.']\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$");
}

bool isInvalidEmail(const QStringView &email) {
    return !std::regex_match(email.toString().toStdString(), EmailIdentifierDetail::emailre);
}

QString randomID(uint8_t len) {
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

QString char32ToQString(const char32_t &c) {
    char32_t alignedChar;
    std::memcpy(&alignedChar, &c, sizeof(char32_t));
    return QString::fromUcs4(&alignedChar, 1);
}