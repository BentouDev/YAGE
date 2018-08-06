#ifndef YAGE_BACKEND_H
#define YAGE_BACKEND_H

#include <QObject>
#include <QString>
#include <QList>
#include <QQmlListProperty>

class Project;

class Backend : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QQmlListProperty<Project> Projects READ GetProjects NOTIFY OnProjectAdded)

public:
    explicit Backend(QObject* parent = nullptr);
    virtual ~Backend();

    // Projects
    void AddProject(const QString& name);    
    QQmlListProperty<Project> GetProjects();

signals:
    void OnProjectAdded();

private:
    QList<Project *> Projects;
};

#endif // YAGE_BACKEND_H 