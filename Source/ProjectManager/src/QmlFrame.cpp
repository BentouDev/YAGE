#include "QmlFrame.h"

#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>

#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFileInfo>
#include <QAction>
#include <QTimer>
#include <QDir>

QmlFrame::QmlFrame(const QString& source, QWidget* parent)
    : QFrame(parent)
{
    _CurrentSource = source;
    _View          = new QQuickView();
    _ViewWidget    = QWidget::createWindowContainer(_View, this);

    _View->setResizeMode(QQuickView::SizeRootObjectToView);
    _ViewWidget->setFocusPolicy(Qt::TabFocus);

    QVBoxLayout* frame = new QVBoxLayout(this);
                 frame->addWidget(_ViewWidget);
                 frame->setSpacing(0);
                 frame->setContentsMargins(0,0,0,0);

    setLayout(frame);

    QAction* action = new QAction("Reload", this);
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    connect(action, &QAction::triggered, [this](){ Reload(); });

    addAction(action);
}

QmlFrame::~QmlFrame()
{

}

void QmlFrame::SetSource(const QString& source)
{
    _CurrentSource = source;
}

auto QmlFrame::GetEngine() -> QQmlEngine*
{
    return _View->engine();
}

void QmlFrame::Reload()
{
    QTimer::singleShot(0, [this]()
    {
        QString   main = QString("Data/Editor/Resources/%1").arg(_CurrentSource);
        QFileInfo exists(main);
        QUrl      runtime;

        if (exists.exists() && exists.isFile())
        {
            runtime = main;
            qDebug() << QString("--yage: Found %1!").arg(_CurrentSource);
        }
        else
        {
            runtime = QUrl(QString("qrc:/%1").arg(_CurrentSource));
        }

        _View->engine()->clearComponentCache();
        _View->setSource(runtime);
    });
}