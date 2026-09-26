#pragma once

#include "products.hpp"
#include <QQmlPropertyMap>
#include <QVariantList>
#include <qqmlregistration.h>

// C++
struct CoreStatus {
    Q_DECLARE_TR_FUNCTIONS(CoreStatus)

private:
    QHash<QString, Kit *> loadedKits;

    CoreStatus()  = default;
    ~CoreStatus() = default;

public:
    CoreStatus(const CoreStatus &)            = delete;
    CoreStatus &operator=(const CoreStatus &) = delete;

    static CoreStatus &instance();

    auto getLoadedKits() const noexcept -> const QHash<QString, Kit *> &;
    bool registerKit(Kit &newKit) noexcept;     // True for success or false if it exists
    bool removeKit(const QString &id) noexcept; // True for success or false if it doesn't exist
    bool containsKit(const QString &id) const noexcept;
    Kit &getKit(const QString &id) const;
};

// QML ViewModel
class QmlStatus : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    // Every loaded kit as { "id": <kit id>, "name": <kit name> }, sorted by name
    Q_PROPERTY(QVariantList loadedKits READ loadedKits NOTIFY loadedKitsChanged)

public:
    Q_INVOKABLE bool registerKit(Kit *newKit) noexcept;
    Q_INVOKABLE bool removeKit(const QString &id) noexcept;
    Q_INVOKABLE bool containsKit(const QString &id) const noexcept;

    QVariantList loadedKits() const;
    // For kits which are loaded / changed by the C++ side (not through this class)
    Q_INVOKABLE void refreshLoadedKits() noexcept;

signals:
    void loadedKitsChanged();
};
