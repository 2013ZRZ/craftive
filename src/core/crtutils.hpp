#pragma once

#include <QCoreApplication>
#include <QList>
#include <QString>
#include <compare>
#include <concepts>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

template <typename T>
concept isJson = std::same_as<std::decay_t<T>, json>;

struct Version {
    Q_DECLARE_TR_FUNCTIONS(Version)

public:
    uint8_t major{0}, minor{1}, patch{0};

    Version() = default; // Default version is the first one "0.1.0"
    Version(uint8_t ma, uint8_t mi, uint8_t p);
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

namespace IDSeparatorDetail {
struct SeparatedElemID {
    QString kit, elem;
};
} // namespace IDSeparatorDetail

auto separateElemID(const QString &s) -> IDSeparatorDetail::SeparatedElemID;

NLOHMANN_JSON_NAMESPACE_BEGIN
template <> struct adl_serializer<QString> {
    static void to_json(json &j, const QString &opt) { j = opt.toStdString(); }
    static void from_json(const json &j, QString &opt) {
        opt = QString::fromStdString(j.get<std::string>());
    }
};

template <typename T> struct adl_serializer<QList<T>> {
    static void to_json(json &j, const QList<T> &opt) {
        j = json::array();
        for (const auto &v : opt) j.push_back(v);
    }
    static void from_json(const json &j, QList<T> &opt) {
        opt.clear();
        opt.reserve(j.size());
        for (const auto &v : j) opt.push_back(v.get<T>());
    }
};
NLOHMANN_JSON_NAMESPACE_END
