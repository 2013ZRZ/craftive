#pragma once

#include <QtCore/QList>
#include <QtCore/QString>
#include <concepts>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool    isInvalidID(const QStringView &id);
bool    isInvalidEmail(const QStringView &email);
QString randomID(uint8_t len = 50);
QString char32ToQString(const char32_t &c);

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

template <typename T>
concept is_string =
    std::same_as<std::decay_t<T>, char *> || std::same_as<std::decay_t<T>, std::string> ||
    std::same_as<std::decay_t<T>, std::string_view> || std::same_as<std::decay_t<T>, QChar *> ||
    std::same_as<std::decay_t<T>, QString> || std::same_as<std::decay_t<T>, QStringView>;

template <typename T>
concept is_json = std::same_as<std::decay_t<T>, json>;