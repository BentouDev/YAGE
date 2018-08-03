#ifndef YAGE_BACKEND_H
#define YAGE_BACKEND_H

#include <QObject>
#include <QString>

class Backend : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    explicit Backend(QObject* parent = nullptr);
    virtual ~Backend();
    
    QString userName();
    void setUserName(const QString &userName);

signals:
    void userNameChanged();

private:
    QString m_userName;
};

#endif // YAGE_BACKEND_H