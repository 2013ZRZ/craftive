#include "crtutils.hpp"
#include <QtCore/QRegularExpression>
#include <ctime>
#include <random>

bool isInvalidID(const QString &id) {
    for (const auto it : id.toStdString()) {
        if (!isalnum(it) && it != '_')
            return true;
    }
    return false;
}

namespace EmailIdentifierDetail {
static const QRegularExpression emailre{
    "\\A\\w+([-+.']\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*\\z"};
}

bool isInvalidEmail(const QString &email) {
    return !EmailIdentifierDetail::emailre.match(email).hasMatch();
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