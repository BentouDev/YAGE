#ifndef YAGE_PROJECT_H
#define YAGE_PROJECT_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QDateTime>

class Project : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QString   Name READ GetName WRITE SetName NOTIFY NameChanged)
    Q_PROPERTY(QDir      Path READ GetPath WRITE SetPath NOTIFY PathChanged)
    Q_PROPERTY(QDateTime Date READ GetDate WRITE SetDate NOTIFY DateChanged)

public:
    explicit Project(QObject* parent = nullptr);
    virtual ~Project();
    
    QString GetName() { return Name; }
    void SetName(const QString &userName);

    QDir GetPath() { return Path; }
    void SetPath(const QDir& path);

    QDateTime GetDate() { return LastUsed; }
    void SetDate(const QDateTime& date);

signals:
    void NameChanged();
    void PathChanged();
    void DateChanged();

private:
    QString   Name;
    QDir      Path;
    QDateTime LastUsed;
};

#endif // YAGE_PROJECT_H