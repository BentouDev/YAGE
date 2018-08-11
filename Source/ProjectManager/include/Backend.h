#ifndef YAGE_BACKEND_H
#define YAGE_BACKEND_H

#include <QQmlListProperty>
#include <QObject>
#include <QString>
#include <QList>
#include <QDir>

class Project;

class Backend : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QQmlListProperty<Project> Projects READ GetProjects NOTIFY OnProjectAdded)
    Q_PROPERTY(QString CurrentDir READ GetCurrentDir NOTIFY OnCurrentDirChanged)

public:
    explicit Backend(QObject* parent = nullptr);
    virtual ~Backend();

    auto GetCurrentDir() -> QString;

    // Projects
    void OpenProject(const QDir& path);
    void OpenProject(Project* project);
    void AddProject(const QString& name);
    auto GetProjects() -> QQmlListProperty<Project>;
    auto GetProjectTemplates() -> QStringList&;

    // Interface
    Q_INVOKABLE void OnPickFolder();
    Q_INVOKABLE bool OnNewProject(const QString& name, int templateIndex = -1);
    Q_INVOKABLE void OnOpenProject();
    Q_INVOKABLE void OnCheckUpdates();
    Q_INVOKABLE void OnAbout();

signals:
    void OnProjectAdded();
    void OnCurrentDirChanged();

private:
    void LoadTemplates();
    void LoadSettings();
    void SaveSettings();
    void CreateProjectFiles(const Project* project, const QString& templatePath);

    QDir            CurrentFolder;
    QList<Project*> Projects;
    QStringList     Templates;
};

#endif // YAGE_BACKEND_H 