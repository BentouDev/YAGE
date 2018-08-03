#include "Backend.h"
#include "Project.h"

Backend::Backend(QObject *parent) :
    QObject(parent)
{
}

Backend::~Backend()
{
    
}

void Backend::AddProject(const QString& name)
{
    auto* proj = new Project();
    proj->SetName(name);
    proj->SetPath("Data/" + name);
    proj->SetDate(QDateTime::currentDateTime());

    emit OnProjectAdded();
}

QQmlListProperty<Project> Backend::GetProjects()
{
    return QQmlListProperty<Project>(this, Projects);
}