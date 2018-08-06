#include <QApplication>
#include "MainWindow.h"
#include "Backend.h"
#include "Project.h"
#include <QStyle>

int main(int argc, char** args)
{
	QApplication app(argc, args);
	
    qmlRegisterType<Backend>("Yage.ProjectManager.Backend", 1, 0, "Backend");
	qmlRegisterType<Project>("Yage.ProjectManager.Backend", 1, 0, "Project");

	Backend backend;
    
    MainWindow window;
	window.show();
	
	window.RegisterBackend(backend);
	
	window.ReloadUI();
	
	backend.AddProject("Test1");
	backend.AddProject("WitchKnight");
	backend.AddProject("RobbutAssault");
	backend.AddProject("SoulOfMagic");
	

	return app.exec();
}