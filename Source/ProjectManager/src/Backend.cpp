#include "Backend.h"
#include "Project.h"

#include <QApplication>
#include <QFileDialog>
#include <QSettings>
#include <QFileInfo>

Backend::Backend(QObject *parent) :
    QObject(parent)
{
    LoadSettings();
}

Backend::~Backend()
{
    SaveSettings();

    for (int i = 0; i < Projects.size(); i++)
    {
        delete Projects[i];
    }

    Projects.clear();
}

void Backend::LoadSettings()
{
    QSettings   settings(QSettings::IniFormat, QSettings::UserScope, 
                         "BentouDev", "YageProjectManager");
    int count = settings.beginReadArray("RecentProjects");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        QString   path = settings.value("Path").toString();
        QDateTime date = settings.value("Date").toDateTime();

        QFileInfo file(path);

        if (!file.exists() || !file.isDir())
            continue;

        auto* proj = new Project(this);
              proj->SetName(file.dir().dirName());
              proj->SetPath(path);
              proj->SetDate(date);
        Projects.push_back(proj);
    }

    settings.endArray();
}

void Backend::SaveSettings()
{
    QSettings   settings(QSettings::IniFormat, QSettings::UserScope, 
                        "BentouDev", "YageProjectManager");
                settings.beginWriteArray("RecentProjects");

    for (int i = 0; i < Projects.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("Path", Projects[i]->GetPath());
        settings.setValue("Date", Projects[i]->GetDateTime());
    }

    settings.endArray();
    settings.sync();
}

void Backend::OpenProject(const QDir& dir)
{
    auto itr = std::find_if(Projects.begin(), Projects.end(), [&](Project* proj){
        return proj->GetPath().compare(dir.absolutePath()) == 0;
    });

    Project* proj = nullptr;

    if (itr != Projects.end())
    {
        proj = (*itr);
    }
    else
    {
        proj = new Project(this);
        proj->SetName(dir.dirName());
        proj->SetPath(dir);
    
        Projects.push_back(proj);

        emit OnProjectAdded();
    }

    OpenProject(proj);
}

void Backend::OpenProject(Project *project)
{
    project->SetDate(QDateTime::currentDateTime());

    QApplication::quit();
}

QQmlListProperty<Project> Backend::GetProjects()
{
    return QQmlListProperty<Project>(this, Projects);
}

void Backend::OnNewProject()
{

}

void Backend::OnOpenProject()
{
    QString dir = QFileDialog::getExistingDirectory
    (
        nullptr,
        tr("Open Directory"),
        "/home",
        QFileDialog::ShowDirsOnly
    );

    if (dir.isEmpty())
        return;

    OpenProject(dir);
}

void Backend::OnCheckUpdates()
{

}

void Backend::OnAbout()
{

}