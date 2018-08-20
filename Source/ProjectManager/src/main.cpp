#include "MainWindow.h"
#include "Backend.h"
#include "Project.h"

#include <QtQml/QQmlEngine>
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>

int main(int argc, char** args)
{
	QApplication app(argc, args);
	app.setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, QColor(33,33,33));
	darkPalette.setColor(QPalette::Dark, QColor(20,20,20));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(53,53,53));
	darkPalette.setColor(QPalette::AlternateBase, QColor(64,64,64));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Foreground, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53,53,53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

	darkPalette.setColor(QPalette::Highlight, QColor(130, 42, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	app.setPalette(darkPalette);

	app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

	qmlRegisterType<Backend>("Yage.ProjectManager.Backend", 1, 0, "Backend");
	qmlRegisterType<Project>("Yage.ProjectManager.Backend", 1, 0, "Project");

	Backend    backend;
	MainWindow window;

	window.RegisterBackend(backend);

	return app.exec();
}