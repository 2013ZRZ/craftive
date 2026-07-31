#pragma once

#include <QObject>
#include <QString>
#include <cstdint>
#include <qqmlregistration.h>
#include <type_traits>

// Craftive Exceptions
class CrtExcept : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString how READ how CONSTANT)
    Q_PROPERTY(QString what READ what CONSTANT)
    Q_PROPERTY(QString whichStr READ whichStr CONSTANT)
    QML_ELEMENT

private:
    const uint16_t code; // Error Code (0x0000 ~ 0xFFFF)
    const QString  detail;

private:
    template <typename T> static auto toArg(T &&t) -> QString {
        if constexpr (std::is_arithmetic_v<std::decay_t<T>>)
            return QString::number(std::forward<T>(t));
        else
            return QString(std::forward<T>(t));
    }

public:
    CrtExcept(const uint16_t _code, const QString &_detail, auto &&...args)
        requires(sizeof...(args) > 0)
        : code(_code), detail(_detail.arg(toArg(args)...)) {}
    CrtExcept(const uint16_t _code, const QString &_detail) : code(_code), detail(_detail) {}

    auto how() const noexcept -> const QString &; // Return detail
    auto what() const noexcept -> QString;        // Return errmsgs[code] (translated)
    auto which() const noexcept -> uint16_t;      // Return code
    auto whichStr() const -> QString;             // Return code as QString
};
