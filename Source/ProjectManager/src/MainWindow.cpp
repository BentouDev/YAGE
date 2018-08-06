#include "MainWindow.h"
#include "Backend.h"
#include "Project.h"
#include "StandardIconProvider.h"
#include <QTimer>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QSizePolicy>
#include <QLayout>
#include <QAction>
#include <QDir>

MainWindow::MainWindow()
{
    setWindowTitle("YAGE - Project Manager");
    resize(600, 800);
    setMinimumSize(480, 200);

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
        _View->engine()->clearComponentCache();
        _View->setSource(QUrl("Data/Editor/main.qml"));
    });
}

void MainWindow::RegisterBackend(Backend& backend)
{
    _View->engine()->rootContext()->setContextProperty("backend", &backend);
    _View->engine()->addImageProvider(QLatin1String("standardIcon"), new StandardIconProvider);
    ReloadUI();
}