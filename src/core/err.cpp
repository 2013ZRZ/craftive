#include "err.hpp"
#include "qcoreapplication.h"
#include <QCoreApplication>

constexpr const char *errmsgs[] = {
    QT_TRANSLATE_NOOP("errmsgs", "Extern error"),
    QT_TRANSLATE_NOOP("errmsgs", "Empty ID"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid ID"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid unicode colored character"),
    QT_TRANSLATE_NOOP("errmsgs", "No such element in this kit"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid location in this large-block"),
    QT_TRANSLATE_NOOP("errmsgs", "Failed to parse JSON"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid email address"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid RGB color"),
    QT_TRANSLATE_NOOP("errmsgs", "Author doesn't exist"),
    QT_TRANSLATE_NOOP("errmsgs", "Data doesn't exist"),
    QT_TRANSLATE_NOOP("errmsgs", "No such kit in all loaded kits"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid Element ID; it should be like \"kit/elem\""),
    QT_TRANSLATE_NOOP("errmsgs", "Product version doesn't exist"),
    QT_TRANSLATE_NOOP("errmsgs", "Invalid location in this map")};

auto CrtExcept::how() const noexcept -> const QString & { return detail; }

auto CrtExcept::what() const noexcept -> QString {
    return QCoreApplication::translate("errmsgs",
                                       code < std::size(errmsgs) ? errmsgs[code] : "Unknown error");
}

auto CrtExcept::which() const noexcept -> QString {
    return QString{"0x%1"}.arg(code, 4, 16, QChar{'0'});
}

bool CrtExcept::isReported() const noexcept { return reported; }

void CrtExcept::markReported() const noexcept { reported = true; }

void CrtExcept::report(const CrtExcept &e) {
    if (e.isReported())
        return;
    else {
        e.markReported();
        QCoreApplication::postEvent(globalExceptReceiver, new CrtExceptEvent{e});
    }
}

auto CrtExceptEvent::how() const noexcept -> const QString & { return m_e.how(); }

auto CrtExceptEvent::what() const noexcept -> QString { return m_e.what(); }

auto CrtExceptEvent::which() const noexcept -> QString { return m_e.which(); }

bool CrtExceptReceiver::event(QEvent *event) {
    if (event->type() == CrtExceptEvent::EventType) {
        auto *ee = static_cast<CrtExceptEvent *>(event);
        emit  exceptionOccurred(ee->which(), ee->what(), ee->how());
        return true;
    }
    return QObject::event(event);
}
