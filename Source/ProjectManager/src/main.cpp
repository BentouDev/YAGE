#include <QApplication>
#include "MainWindow.h"
#include "Backend.h"

int main(int argc, char** args)
{
	QApplication a(argc, args);
	
    qmlRegisterType<Backend>("Yage.ProjectManager.Backend", 1, 0, "Backend");
    
    MainWindow w;
	w.show();

	return a.exec();
}