#include "Project.h"

Project::Project(QObject* parent)
    : QObject(parent)
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