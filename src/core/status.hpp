#pragma once

#include "elements.hpp"

struct CoreStatus {
    friend class Kit;

  private:
    QHash<QStrPtr, Kit *> loadedKits;

    CoreStatus()  = default;
    ~CoreStatus() = default;

  public:
    CoreStatus(const CoreStatus &)            = delete;
    CoreStatus &operator=(const CoreStatus &) = delete;

    static CoreStatus &instance();

    auto getLoadedKits() const noexcept -> const QHash<QStrPtr, Kit *> &;
    bool registerKit(Kit &newKit) noexcept; // True for success or false if it exists
    bool removeKit(QString &id) noexcept;   // True for success or false if it doesn't exist
    bool containsKit(QString &id) const noexcept;
    Kit &getKit(QString &id) const;
};