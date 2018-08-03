#include "MainWindow.h"
#include "Backend.h"
#include <QTimer>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QLayout>
#include <QAction>
#include <QDir>

MainWindow::MainWindow()
{
    setWindowTitle("YAGE - Project Manager");
    resize(600,800);

    qDebug() << "-- yage: Working dir: " << QDir::currentPath();

    _View       = new QQuickView();
    _ViewWidget = QWidget::createWindowContainer(_View, this);

    _ViewWidget->setFocusPolicy(Qt::TabFocus);

    layout()->addWidget(_ViewWidget);

    QAction* action = new QAction("Reload", this);
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    connect(action, &QAction::triggered, [this](){ReloadUI();});

    addAction(action);

    ReloadUI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    _ViewWidget->setMinimumSize(event->size());
    _ViewWidget->setMaximumSize(event->size());
}

void MainWindow::ReloadUI()
{
    QTimer::singleShot(100, [this](){
        _View->engine()->clearComponentCache();
        _View->setSource(QUrl("Data/main.qml"));
    });
}

Q_DECLARE_METATYPE(QQmlListProperty<Project>);

void MainWindow::RegisterBackend(Backend& backend)
{
    _View->engine()->rootContext()->setContextProperty("projects", QVariant::fromValue(backend.GetProjects()));
    ReloadUI();
}