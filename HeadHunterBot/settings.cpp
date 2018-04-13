#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
     mIDResume = "";
     mHhtoken = "";
     mHhuid = "";
     mXsrf = "";
}

QString Settings::parseDataToJSON()
{
    QJsonObject jsonObj;
    jsonObj["IDResume"] = this->mIDResume;
    jsonObj["Hhtoken"] = this->mHhtoken;
    jsonObj["Hhuid"] = this->mHhuid;
    jsonObj["Xsrf"] = this->mXsrf;

    QJsonDocument doc(jsonObj);

    QString result(doc.toJson(QJsonDocument::Indented));

    return result;
}

void Settings::parseDataFromJSON(QString const & data)
{
    QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());

    QJsonObject jsonObj = document.object();
    this->mIDResume = jsonObj["IDResume"].toString();
    this->mHhtoken = jsonObj["Hhtoken"].toString();
    this->mHhuid = jsonObj["Hhuid"].toString();
    this->mXsrf = jsonObj["Xsrf"].toString();
}

void Settings::loadSettings(QString const & pathToLoad)
{
    QString result = "";
    QString temp = "";

    QFile file;
    file.setFileName(pathToLoad);
    if (file.open(QFile::ReadWrite))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            in >> temp;
            result += temp;
        }
    }

    parseDataFromJSON(result);
}

void Settings::saveSettings(QString const & pathToSave)
{
    QString result = parseDataToJSON();

    QFile file;
    file.setFileName(pathToSave + ".json");
    if (file.open(QFile::ReadWrite))
    {
        QTextStream out(&file);

        out << result;
    }
    file.close();
}

QString const & Settings::getIDResume()
{
    return this->mIDResume;
}

QString const & Settings::getHhtoken()
{
    return this->mHhtoken;
}

QString const & Settings::getHhuid()
{
    return this->mHhuid;
}

QString const & Settings::getXsrf()
{
    return this->mXsrf;
}

void Settings::setIDResume(QString const & data)
{
    this->mIDResume = data;
}

void Settings::setHhtoken(QString const & data)
{
    this->mHhtoken = data;
}

void Settings::setHhuid(QString const & data)
{
    this->mHhuid = data;
}

void Settings::setXsrf(QString const & data)
{
    this->mXsrf = data;
}
