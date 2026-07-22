#include "status.hpp"
#include "err.hpp"

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
    if (loadedKits.contains(id))
        return *(loadedKits[id]);
    else
        throw CrtExcept(0x000B,
                        tr("from CoreStatus::getKit(); no one's ID is %1 in all loaded kits, "
                           "please load this kit and try again"),
                        id);
}
