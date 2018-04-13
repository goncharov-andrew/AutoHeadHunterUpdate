#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    void loadSettings(QString const & pathToLoad);

    void saveSettings(QString const & pathToSave);

    QString const & getIDResume();

    QString const & getHhtoken();

    QString const & getHhuid();

    QString const & getXsrf();

    void setIDResume(QString const & data);

    void setHhtoken(QString const & data);

    void setHhuid(QString const & data);

    void setXsrf(QString const & data);

signals:

public slots:

private:

    QString parseDataToJSON();

    void parseDataFromJSON(QString const & data);

    QString mIDResume;
    QString mHhtoken;
    QString mHhuid;
    QString mXsrf;
};

#endif // SETTINGS_H
