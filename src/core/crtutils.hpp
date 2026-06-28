#pragma once

#include <QtCore/QList>
#include <QtCore/QString>
#include <compare>
#include <concepts>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

template <typename T>
concept isStr =
    std::same_as<std::decay_t<T>, char *> || std::same_as<std::decay_t<T>, std::string> ||
    std::same_as<std::decay_t<T>, std::string_view> || std::same_as<std::decay_t<T>, QChar *> ||
    std::same_as<std::decay_t<T>, QString> || std::same_as<std::decay_t<T>, QStringView>;

template <typename T>
concept isJson = std::same_as<std::decay_t<T>, json>;

struct Version {
    uint8_t major{0}, minor{1}, patch{0};

    Version() : major(0), minor(1), patch(0) {} // Default version is the first one "0.1.0"
    Version(uint8_t ma, uint8_t mi, uint8_t p) : major(ma), minor(mi), patch(p) {}
    explicit Version(const isJson auto &j);

    auto    operator<=>(const Version &other) const noexcept -> std::strong_ordering;
    bool    operator==(const Version &other) const noexcept;
    QString operator()() const;
    void    fromJson(const json &j);
    json    toJson() const;
};

bool    isInvalidID(const QString &id);
bool    isInvalidElemID(const QString &id);
bool    isInvalidEmail(const QString &email);
QString randomID(uint8_t len = 50);

/** @param s    element ID like "kit/elem"
 *  @param part 0 for kit or 1 for elem
 */
QString separateElemID(const QString &s, bool part);

NLOHMANN_JSON_NAMESPACE_BEGIN
template <> struct adl_serializer<QString> {
    static void to_json(json &j, const QString &opt) { j = opt.toStdString(); }
    static void from_json(const json &j, QString &opt) {
        opt = QString::fromStdString(j.get<std::string>());
    }
};

template <typename T> struct adl_serializer<QList<T>> {
    static void to_json(json &j, const QList<T> &opt) { j = opt.toStdVector(); }
    static void from_json(const json &j, QString &opt) {
        opt = QList<T>::fromStdVector(j.get<std::string>());
    }
};
NLOHMANN_JSON_NAMESPACE_END
