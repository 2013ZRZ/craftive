#include "status.hpp"
#include "err.hpp"
#include <QVariantMap>
#include <algorithm>

CoreStatus &CoreStatus::instance() {
    static CoreStatus inst;
    return inst;
}

auto CoreStatus::getLoadedKits() const noexcept -> const QHash<QString, Kit *> & {
    return loadedKits;
}

bool CoreStatus::registerKit(Kit &newKit) noexcept {
    if (loadedKits.contains(newKit.id))
        return false;
    else {
        loadedKits.insert(newKit.id, &newKit);
        return true;
    }
}

bool CoreStatus::removeKit(const QString &id) noexcept {
    if (loadedKits.contains(id)) {
        loadedKits.remove(id);
        loadedKits.squeeze();
        return true;
    } else
        return false;
}

bool CoreStatus::containsKit(const QString &id) const noexcept { return loadedKits.contains(id); }

Kit &CoreStatus::getKit(const QString &id) const {
    try {
        if (loadedKits.contains(id))
            return *(loadedKits[id]);
        else
            throw CrtExcept(0x000B,
                            tr("From CoreStatus::getKit(); no one's ID is %1 in all loaded kits, "
                               "please load this kit and try again"),
                            id);
    }
    CATCH_THROW(CoreStatus::getKit())
}

bool QmlStatus::registerKit(Kit *newKit) noexcept {
    try {
        if (newKit == nullptr)
            return false;
        if (!CoreStatus::instance().registerKit(*newKit))
            return false;
        emit loadedKitsChanged();
        return true;
    }
    CATCH_AT_ENTRY(QmlStatus::registerKit())
    return {};
}

bool QmlStatus::removeKit(const QString &id) noexcept {
    try {
        if (!CoreStatus::instance().removeKit(id))
            return false;
        emit loadedKitsChanged();
        return true;
    }
    CATCH_AT_ENTRY(QmlStatus::removeKit())
    return {};
}

bool QmlStatus::containsKit(const QString &id) const noexcept {
    try {
        return CoreStatus::instance().containsKit(id);
    }
    CATCH_AT_ENTRY(QmlStatus::containsKit())
    return {};
}

QVariantList QmlStatus::loadedKits() const {
    try {
        const auto  &kits = CoreStatus::instance().getLoadedKits();
        QVariantList result;
        result.reserve(kits.size());
        for (auto it = kits.cbegin(); it != kits.cend(); ++it) {
            if (it.value() == nullptr)
                continue;
            result.append(QVariantMap{{"id", it.key()}, {"name", it.value()->getName()}});
        }
        // QHash doesn't keep any order, so sort them to keep the view stable
        std::sort(result.begin(), result.end(), [](const QVariant &a, const QVariant &b) {
            const auto lhs    = a.toMap();
            const auto rhs    = b.toMap();
            const int  byName = lhs.value("name").toString().compare(rhs.value("name").toString(),
                                                                     Qt::CaseInsensitive);
            return byName != 0 ? byName < 0
                               : lhs.value("id").toString() < rhs.value("id").toString();
        });
        return result;
    }
    CATCH_AT_ENTRY(QmlStatus::loadedKits())
    return {};
}

void QmlStatus::refreshLoadedKits() noexcept {
    try {
        emit loadedKitsChanged();
    }
    CATCH_AT_ENTRY(QmlStatus::refreshLoadedKits())
}
