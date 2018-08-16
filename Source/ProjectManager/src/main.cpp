#include "MainWindow.h"
#include "Backend.h"
#include "Project.h"

#include <QtQml/QQmlEngine>
#include <QApplication>
#include <QStyle>

int main(int argc, char** args)
{
	QApplication app(argc, args);
	
    qmlRegisterType<Backend>("Yage.ProjectManager.Backend", 1, 0, "Backend");
	qmlRegisterType<Project>("Yage.ProjectManager.Backend", 1, 0, "Project");

	Backend    backend;    
    MainWindow window;

	window.RegisterBackend(backend);

	return app.exec();
}