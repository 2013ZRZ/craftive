#include "crtutils.hpp"
#include <QtCore/QRegularExpression>
#include <ctime>
#include <random>

Version::Version(uint8_t ma, uint8_t mi, uint8_t p) : major(ma), minor(mi), patch(p) {}

Version::Version(const isJson auto &j) { fromJson(j); }

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

QString Version::operator()() const { return QString{"%1.%2.%3"}.arg(major, minor, patch); }

void Version::fromJson(const json &j) {
    // j should be an array like [x, y, z] for "x.y.z"
    if (!j.is_array())
        throw CrtExcept(0x0006,
                        translate("Version", "from Version::fromJson(); the version's json isn't an array. It "
                           "should be like [x, y, z] for \"x.y.z\""));
    major = j[0];
    minor = j[1];
    patch = j[2];
}

json Version::toJson() const { return json{major, minor, patch}; }

QStrPtr::QStrPtr() : raw(nullptr) {}

QStrPtr::QStrPtr(QString &s) : raw(&s) {}

QStrPtr::operator QString() const noexcept { return get(); }

QString QStrPtr::get() const noexcept { return raw == nullptr ? QString{} : *raw; }

QString *QStrPtr::operator->() const noexcept { return raw; }

bool QStrPtr::operator==(const QStrPtr &other) const noexcept { return this->get() == other.get(); }

size_t qHash(QStrPtr key, size_t seed) { return qHash(key.get(), seed); }

size_t qHash(const QStrPtr &key, size_t seed) { return qHash(key.get(), seed); }

bool isInvalidID(const QString &id) {
    for (const QChar &ch : id) {
        if (!ch.isLetterOrNumber() && ch != u'_')
            return true;
    }
    return false;
}

bool isInvalidElemID(const QString &id) { return isInvalidID(id) || (id.split(u'/').size() != 2); }

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