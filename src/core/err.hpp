#pragma once

#include "qcoreapplication.h"
#include <QEvent>
#include <QObject>
#include <QString>
#include <cstdint>
#include <exception>
#include <qqmlregistration.h>
#include <type_traits>

// Craftive Exception (for throwing; interact with the event)
class CrtExcept {
private:
    const uint16_t code; // Error Code (0x0000 ~ 0xFFFF)
    const QString  detail;
    mutable bool   reported{false}; // To avoid duplicate report

    template <typename T> static auto toArg(T &&t) -> QString {
        if constexpr (std::is_arithmetic_v<std::decay_t<T>>)
            return QString::number(std::forward<T>(t));
        else
            return QString(std::forward<T>(t));
    }

public:
    CrtExcept() = delete;
    CrtExcept(const uint16_t _code, const QString &_detail, auto &&...args)
        requires(sizeof...(args) > 0)
        : code(_code), detail(_detail.arg(toArg(args)...)) {}
    CrtExcept(const uint16_t _code, const QString &_detail) : code(_code), detail(_detail) {}

    auto how() const noexcept -> const QString &; // Return detail
    auto what() const noexcept -> QString;        // Return errmsgs[code] (translated)
    auto which() const noexcept -> QString;       // Return code as QString
    bool isReported() const noexcept;
    void markReported() const noexcept;

    static void report(const CrtExcept &e); // Report once per exception
};

// Event (Interact with the receiver)
class CrtExceptEvent : public QEvent {
private:
    const CrtExcept m_e;

public:
    static const QEvent::Type EventType = static_cast<QEvent::Type>(QEvent::User + 1);

    CrtExceptEvent(const CrtExcept &e) : QEvent(EventType), m_e(e) {}

    auto how() const noexcept -> const QString &;
    auto what() const noexcept -> QString;
    auto which() const noexcept -> QString;
};

// Receiver (interact with QML frontend)
class CrtExceptReceiver : public QObject {
    Q_OBJECT

public:
    explicit CrtExceptReceiver(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool event(QEvent *event) override;

signals:
    void exceptionOccurred(const QString &which, const QString &what, const QString &how);
};

extern CrtExceptReceiver *globalExceptReceiver;

#define CATCH_THROW(context)                                                               \
    catch (const CrtExcept &e) {                                                           \
        CrtExcept::report(e);                                                              \
        throw;                                                                             \
    }                                                                                      \
    catch (const std::exception &e) {                                                      \
        CrtExcept wrapped{0x0000,                                                          \
                          QCoreApplication::translate(                                     \
                              "errmsgs", "From %1; the original error message is \"%2\""), \
                          #context,                                                        \
                          e.what()};                                                       \
        CrtExcept::report(wrapped);                                                        \
        throw wrapped;                                                                     \
    }                                                                                      \
    catch (...) {                                                                          \
        CrtExcept wrapped{                                                                 \
            0x0000,                                                                        \
            QCoreApplication::translate(                                                   \
                "errmsgs",                                                                 \
                "From %1; unknown exception type, neither CrtExcept nor std::exception"),  \
            #context};                                                                     \
        CrtExcept::report(wrapped);                                                        \
        throw wrapped;                                                                     \
    }

// Only for QML callees
#define CATCH_AT_ENTRY(context)                                                            \
    catch (const CrtExcept &e) {                                                           \
        CrtExcept::report(e);                                                              \
    }                                                                                      \
    catch (const std::exception &e) {                                                      \
        CrtExcept wrapped{0x0000,                                                          \
                          QCoreApplication::translate(                                     \
                              "errmsgs", "From %1; the original error message is \"%2\""), \
                          #context,                                                        \
                          e.what()};                                                       \
        CrtExcept::report(wrapped);                                                        \
    }                                                                                      \
    catch (...) {                                                                          \
        CrtExcept wrapped{                                                                 \
            0x0000,                                                                        \
            QCoreApplication::translate(                                                   \
                "errmsgs",                                                                 \
                "From %1; unknown exception type, neither CrtExcept nor std::exception"),  \
            #context};                                                                     \
        CrtExcept::report(wrapped);                                                        \
    }