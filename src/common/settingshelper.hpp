#pragma once

#include <QColor>
#include <QLocale>
#include <QObject>
#include <QSettings>
#include <qqmlregistration.h>

class SettingsHelper : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    // Items (Name format: group_key)
    // Appearance Section
    Q_PROPERTY(QLocale appearance_locale READ appearance_locale WRITE setAppearance_locale NOTIFY
                   appearance_localeChanged)
    Q_PROPERTY(bool appearance_isDarkTheme READ appearance_isDarkTheme WRITE
                   setAppearance_isDarkTheme NOTIFY appearance_isDarkThemeChanged)
    Q_PROPERTY(QColor appearance_seedColor READ appearance_seedColor WRITE setAppearance_seedColor
                   NOTIFY appearance_seedColorChanged)


    // Utilities
    Q_PROPERTY(QLocale usingLocale READ usingLocale CONSTANT)
    Q_PROPERTY(QList<QLocale> availableLocales READ availableLocales CONSTANT)
    Q_PROPERTY(bool followingSystemLocale READ followingSystemLocale CONSTANT)

private:
    QSettings m_s{"craftive", "craftive"};

public:
    SettingsHelper(QObject *parent = nullptr) : QObject(parent) {}

    // Why QString (instead of QAnyStringView) is that QAnyStringView is incompatible with QML :(
    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue) const;
    Q_INVOKABLE bool     contains(const QString &key) const;
    Q_INVOKABLE void     remove(const QString &key);

    // Apperance Section
public:
    QLocale appearance_locale() const;
    bool    appearance_isDarkTheme() const;
    QColor  appearance_seedColor() const;
    void    setAppearance_locale(const QLocale &v);
    void    setAppearance_isDarkTheme(bool v);
    void    setAppearance_seedColor(const QColor &v);

signals:
    void appearance_localeChanged();
    void appearance_isDarkThemeChanged();
    void appearance_seedColorChanged();


    // Utilities
public:
    QLocale          usingLocale() const;
    QList<QLocale>   availableLocales() const;
    bool             followingSystemLocale() const;
    Q_INVOKABLE void restartApp() const;
};