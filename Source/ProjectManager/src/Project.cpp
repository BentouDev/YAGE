#include "Project.h"
#include "Backend.h"

Project::Project(Backend* backend)
    : QObject(backend)
    , _Backend(backend)
{

}

Project::~Project()
{

}

void Project::SetName(const QString& name)
{
    Name = name;
}

void Project::SetPath(const QDir& path)
{
    Path = path;
}

void Project::SetDate(const QDateTime& date)
{
    LastUsed = date;
}

Q_INVOKABLE void Project::OnOpen()
{
    if (_Backend) _Backend->OpenProject(this);
}