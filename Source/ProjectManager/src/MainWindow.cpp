#include "MainWindow.h"
#include "Backend.h"
#include "Project.h"
#include "QmlFrame.h"
#include "StandardIconProvider.h"

#include <QDebug>
#include <QPixmap>
#include <QSplashScreen>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>

MainWindow::MainWindow()
{
    qDebug() << "-- yage: Working dir: " << QDir::currentPath();

	QSplashScreen* splash = nullptr;
	QPixmap*       pixmap = nullptr;

	QFile image(":/resources/images/splash.png");

	if (image.open(QFile::ReadOnly))
	{
		pixmap = new QPixmap(":/resources/images/splash.png");
		splash = new QSplashScreen(*pixmap);

		//QFont splashFont;
		//splashFont.setFamily("Arial");
		//splashFont.setBold(true);
		//splashFont.setPixelSize(9);
		//splashFont.setStretch(125);

		//splash->setFont(splashFont);

		splash->showMessage("Loading qml...", Qt::AlignBottom | Qt::AlignHCenter, Qt::white);
		splash->show();

		qApp->processEvents();
	}

	setWindowTitle("YAGE - Project Manager");
	resize(600, 400);
	setMinimumSize(600, 200);

	_MainQml = new QmlFrame("MainWindow.qml", this);
	setCentralWidget(_MainQml);

    connect (_MainQml, &QmlFrame::OnLoaded, [=]()
	{
        show();
        
		if (splash)
		{
			splash->finish(this);

			delete splash;
			delete pixmap;
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