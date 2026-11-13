#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QTranslator>

class QQmlEngine;

class Translator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QVariantList languages READ languages CONSTANT)

public:
    explicit Translator(QQmlEngine *engine, QObject *parent = nullptr);

    QString language() const { return m_language; }
    QVariantList languages() const;

    Q_INVOKABLE void setLanguage(const QString &code);

signals:
    void languageChanged();

private:
    bool loadTranslation(const QString &code);

    QQmlEngine *m_engine;
    QTranslator m_translator;
    QString m_language;
};
