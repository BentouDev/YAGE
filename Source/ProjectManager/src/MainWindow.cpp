#include "MainWindow.h"
#include "Backend.h"
#include "Project.h"
#include "QmlFrame.h"
#include "StandardIconProvider.h"

#include <QDebug>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>

MainWindow::MainWindow()
{
    qDebug() << "-- yage: Working dir: " << QDir::currentPath();

    setWindowTitle("YAGE - Project Manager");
    resize(600, 400);
    setMinimumSize(600, 200);

    _MainQml = new QmlFrame("MainWindow.qml", this);
    setCentralWidget(_MainQml);
}

MainWindow::~MainWindow()
{

}

void MainWindow::RegisterBackend(Backend& backend)
{
    _MainQml->GetEngine()->rootContext()->setContextProperty("backend", &backend);
    _MainQml->GetEngine()->rootContext()->setContextProperty("templates", QVariant::fromValue(backend.GetProjectTemplates()));
    _MainQml->GetEngine()->addImageProvider(QLatin1String("standardIcon"), new StandardIconProvider);

    _MainQml->Reload();
}