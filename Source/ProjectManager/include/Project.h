#ifndef YAGE_PROJECT_H
#define YAGE_PROJECT_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QDateTime>

class Backend;

class Project : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QString Name READ GetName NOTIFY NameChanged)
    Q_PROPERTY(QString Path READ GetPath NOTIFY PathChanged)
    Q_PROPERTY(QString Date READ GetDate NOTIFY DateChanged)

public:
    explicit Project(Backend* parent = nullptr);
    virtual ~Project();
    
    QString GetName() const { return Name; }
    void SetName(const QString &userName);

    QString GetPath() const { return Path.absolutePath(); }
    void SetPath(const QDir& path);

    QDateTime GetDateTime() const { return LastUsed; }
    QString   GetDate() const { return LastUsed.toString(); }
    void SetDate(const QDateTime& date);

    // Interface
    Q_INVOKABLE void OnOpen();

signals:
    void NameChanged();
    void PathChanged();
    void DateChanged();

private:
    Backend*  _Backend;

    QString   Name;
    QDir      Path;
    QDateTime LastUsed;
};

#endif // YAGE_PROJECT_H