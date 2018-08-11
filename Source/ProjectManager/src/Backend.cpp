#include "Backend.h"
#include "Project.h"

#include <QStandardPaths>
#include <QDirIterator>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QFileInfo>
#include <QProcess>

Backend::Backend(QObject *parent) :
    QObject(parent)
{
    LoadSettings();
    LoadTemplates();
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

void Backend::LoadTemplates()
{
    QStringList templatePaths = QStandardPaths::locateAll
    (
        QStandardPaths::ConfigLocation,
        "BentouDev",
        QStandardPaths::LocateDirectory
    );

    for (auto itr : templatePaths)
    {
        QDir dir(itr);
             
        if (!dir.cd("ProjectTemplates"))
            continue;

        QDirIterator dirItr
        (
            dir.absolutePath(),
            QDir::Dirs | QDir::NoDotAndDotDot, 
            QDirIterator::Subdirectories
        );

        while (dirItr.hasNext())
        {
            dirItr.next();

            if (!dirItr.fileInfo().dir().isEmpty())
            {
                Templates.push_back(dirItr.filePath());
            }
        }
    }
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

        auto itr = std::find_if(Projects.begin(), Projects.end(), [&](Project* proj){
            return proj->GetPath().compare(path) == 0;
        });

        if (itr != Projects.end())
            continue;

        auto* proj = new Project(this);
              proj->SetName(file.baseName());
              proj->SetPath(path);
              proj->SetDate(date);
        Projects.push_back(proj);
    }

    settings.endArray();

    std::sort(Projects.begin(), Projects.end(), [](const Project* first, const Project* second){
        return first->GetDateTime() > second->GetDateTime();
    });
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

auto Backend::GetCurrentDir() -> QString
{
    return CurrentFolder.absolutePath();
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

    QStringList args;
                args.push_back(project->GetPath());

    LaunchYageEditor(args);

    QApplication::quit();
}

QQmlListProperty<Project> Backend::GetProjects()
{
    return QQmlListProperty<Project>(this, Projects);
}

auto Backend::GetProjectTemplates() -> QStringList&
{
    return Templates;
}

void Backend::OnPickFolder()
{
    QString dir = QFileDialog::getExistingDirectory
    (
        nullptr,
        tr("Open Directory"),
        CurrentFolder.absolutePath(),
        QFileDialog::ShowDirsOnly
    );

    QDir new_dir(dir);

    if (!new_dir.exists())
        return;

    CurrentFolder = new_dir;
    emit OnCurrentDirChanged();
}

bool Backend::OnNewProject(const QString& name, int templateIndex)
{
    if (name.isEmpty())
        return false;

    if (!CurrentFolder.exists())
        return false;

    if (!CurrentFolder.cd(name))
    {
        CurrentFolder.mkdir(name);

        if (!CurrentFolder.cd(name))
            return false;
    }

    if (!CurrentFolder.isEmpty())
    {
        QMessageBox choice;
        choice.setText(
            QString("Selected directory '%1' is not empty! Do you want to create anyway?")
            .arg(CurrentFolder.absolutePath())
        );

        choice.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        choice.setDefaultButton(QMessageBox::Cancel);

        if (choice.exec() != QMessageBox::Ok)
        {
            CurrentFolder.cdUp();
            return false;
        }
    }

    auto* proj = new Project();
    proj->SetName(name);
    proj->SetPath(CurrentFolder);

    Projects.push_back(proj);
    emit OnProjectAdded();

    if (templateIndex >= 0 && templateIndex < Templates.size())
        CreateProjectFiles(proj, Templates[templateIndex]);

    OpenProject(proj);

    return true;
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
    QMessageBox dialog;
    dialog.setWindowTitle("YAGE - Check for updates");
    dialog.setText("Checking for updates is not implemented yet!");
    dialog.setIcon(QMessageBox::Warning);
    dialog.exec();
}

void Backend::OnAbout()
{
    QMessageBox::about(
        nullptr,
        "YAGE - About",
        "<h3>Yet Another Game Engine</h3>\n"
        "<p>by BentouDev</p>\n"
        "<p><a href='https://github.com/BentouDev/YAGE'>github</a></p>"
    );
}

namespace detail
{
    bool copyRecursively(const QString sourceFolder, const QString destFolder)
    {
        bool success = false;
        QDir sourceDir(sourceFolder);

        if(!sourceDir.exists())
            return false;

        QDir destDir(destFolder);
        if(!destDir.exists())
            destDir.mkdir(destFolder);

        QStringList files = sourceDir.entryList(QDir::Files);
        for(int i = 0; i< files.count(); i++) {
            QString srcName = sourceFolder + QDir::separator() + files[i];
            QString destName = destFolder + QDir::separator() + files[i];
            success = QFile::copy(srcName, destName);
            if(!success)
                return false;
        }

        files.clear();
        files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
        for(int i = 0; i< files.count(); i++)
        {
            QString srcName = sourceFolder + QDir::separator() + files[i];
            QString destName = destFolder + QDir::separator() + files[i];
            success = copyRecursively(srcName, destName);
            if(!success)
                return false;
        }

        return true;
    }
}

void Backend::CreateProjectFiles(const Project* project, const QString& templatePath)
{
    detail::copyRecursively(templatePath, project->GetPath());

    // Execute mustache
}

void Backend::LaunchYageEditor(const QStringList& args)
{
    QProcess::startDetached("YageEditor", args);
}