#pragma once

#include "elements.hpp"

struct CoreStatus {
  private:
    CoreStatus()  = default;
    ~CoreStatus() = default;

  public:
    QList<Kit *> loadedKits;

    CoreStatus(const CoreStatus &)            = delete;
    CoreStatus &operator=(const CoreStatus &) = delete;

    static CoreStatus &instance();
};