#include "settingshelper.hpp"
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <algorithm>

QVariant SettingsHelper::value(const QString &key, const QVariant &defaultValue) const {
    return m_s.value(key, defaultValue);
}
bool SettingsHelper::contains(const QString &key) const { return m_s.contains(key); }
void SettingsHelper::remove(const QString &key) { m_s.remove(key); }

// Appearance Section
QLocale SettingsHelper::appearance_locale() const {
    return m_s.value("appearance/locale").toLocale();
}
bool SettingsHelper::appearance_isDarkTheme() const {
    return m_s.value("appearance/isDarkTheme").toBool();
}
QColor SettingsHelper::appearance_seedColor() const {
    return m_s.value("appearance/seedColor").value<QColor>();
}
void SettingsHelper::setAppearance_locale(const QLocale &v) {
    if (v != appearance_locale()) {
        m_s.setValue("appearance/locale", v);
        emit appearance_localeChanged();
    }
}
void SettingsHelper::setAppearance_isDarkTheme(bool v) {
    if (v != appearance_isDarkTheme()) {
        m_s.setValue("appearance/isDarkTheme", v);
        emit appearance_isDarkThemeChanged();
    }
}
void SettingsHelper::setAppearance_seedColor(const QColor &v) {
    if (v != appearance_seedColor()) {
        m_s.setValue("appearance/seedColor", v);
        emit appearance_seedColorChanged();
    }
}

// Utilities
QLocale SettingsHelper::usingLocale() const {
    return followingSystemLocale() ? QLocale::system() : appearance_locale();
}
QList<QLocale> SettingsHelper::availableLocales() const {
    QList<QLocale> result;
    auto           qms = QDir{":/i18n"}.entryList({"craftive_*.qm"}, QDir::Files);
    for (auto &name : qms) {
        const QLocale locale{name.remove(0, name.indexOf(u'_') + 1).chopped(3)};
        if (locale.language() == QLocale::C)
            continue;
        // Drop duplicates: e.g. "en" and "en_US" both normalize to the same locale.
        const bool duplicate =
            std::any_of(result.cbegin(), result.cend(), [&locale](const QLocale &l) {
                return l.name() == locale.name();
            });
        if (!duplicate)
            result.append(locale);
    }
    result.emplaceFront(QLocale::system());
    return result;
}
bool SettingsHelper::followingSystemLocale() const { return !m_s.contains("appearance/locale"); }
void SettingsHelper::restartApp() const {
    QProcess::startDetached(QCoreApplication::applicationFilePath(), QCoreApplication::arguments());
    QCoreApplication::quit();
}