#include "MainWindow.h"
#include "Backend.h"
#include "Project.h"
#include "QmlFrame.h"
#include "StandardIconProvider.h"

#include <QDebug>
#include <QPixmap>
#include <QPointer>
#include <QSplashScreen>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>

template <typename T>
void SAFE_DELETE(T*& ptr)
{
    delete ptr;
    ptr = nullptr;
}

MainWindow::MainWindow()
{
    qDebug() << "-- yage: Working dir: " << QDir::currentPath();

    QFile image(":/resources/images/splash.png");

    if (image.open(QFile::ReadOnly))
    {
        _SplashImage = new QPixmap(":/resources/images/splash.png");
        _Splash      = new QSplashScreen(*_SplashImage);

        //QFont splashFont;
        //splashFont.setFamily("Arial");
        //splashFont.setBold(true);
        //splashFont.setPixelSize(9);
        //splashFont.setStretch(125);

        //splash->setFont(splashFont);

        _Splash->showMessage("Loading qml...", Qt::AlignBottom | Qt::AlignHCenter, Qt::white);
        _Splash->show();

        qApp->processEvents();
    }

    setWindowTitle("YAGE - Project Manager");
    resize(600, 400);
    setMinimumSize(600, 200);

    _MainQml = new QmlFrame("MainWindow.qml", this);
    setCentralWidget(_MainQml);

    connect (_MainQml, &QmlFrame::OnLoaded, [this]()
    {
        show();
        
        if (_Splash)
        {
            _Splash->finish(this);

            SAFE_DELETE(_Splash);
            SAFE_DELETE(_SplashImage);
        }
    });
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