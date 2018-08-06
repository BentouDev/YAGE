#include "MainWindow.h"
#include "Backend.h"
#include "Project.h"
#include "StandardIconProvider.h"

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>

#include <QSizePolicy>
#include <QFileInfo>
#include <QLayout>
#include <QAction>
#include <QTimer>
#include <QDir>

MainWindow::MainWindow()
{
    setWindowTitle("YAGE - Project Manager");
    resize(600, 400);
    setMinimumSize(600, 200);

    qDebug() << "-- yage: Working dir: " << QDir::currentPath();

    _View       = new QQuickView();
    _ViewWidget = QWidget::createWindowContainer(_View, this);
    _ViewWidget->setFocusPolicy(Qt::TabFocus);
    _View->setResizeMode(QQuickView::SizeRootObjectToView);

    setCentralWidget(_ViewWidget);

    QAction* action = new QAction("Reload", this);
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    connect(action, &QAction::triggered, [this](){ReloadUI();});

    addAction(action);
}

MainWindow::~MainWindow()
{

}

void MainWindow::ReloadUI()
{
    QTimer::singleShot(0, [this](){
        QString   main("Data/Editor/main.qml");
        QFileInfo exists(main);
        QUrl      runtime;

        if (exists.exists() && exists.isFile())
        {
            runtime = main;
            qDebug() << "--yage: Found main.qml!";
        }
        else
        {
            runtime = QUrl("qrc:/main.qml");
        }

        _View->engine()->clearComponentCache();
        _View->setSource(runtime);
    });
}

void MainWindow::RegisterBackend(Backend& backend)
{
    _View->engine()->rootContext()->setContextProperty("backend", &backend);
    _View->engine()->addImageProvider(QLatin1String("standardIcon"), new StandardIconProvider);
    ReloadUI();
}