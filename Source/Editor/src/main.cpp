//
// Created by bentoo on 10/24/16.
//
#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include "EditorWindow.h"

static void initStyleSheet(QApplication& a)
{
	// Q_INIT_RESOURCE(ads); // If static linked.
	QFile f(":/resources/stylesheets/default-windows.css");
	if (f.open(QFile::ReadOnly))
	{
		const QByteArray ba = f.readAll();
		f.close();
		a.setStyleSheet(QString(ba));
	}
}

int main(int argc, char** args)
{
	QApplication a(argc, args);
	//a.setStyle(QStyleFactory::create("Fusion"));

	/*QFile stylesheet( ":/resources/EditorWindow.css" );
	if(stylesheet.open(QFile::ReadOnly))
	{
		QString styleSheet = stylesheet.readAll();
		a.setStyleSheet( styleSheet );
	}*/

	a.setStyle(QStyleFactory::create("Fusion"));

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

	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	a.setPalette(darkPalette);

	a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

	initStyleSheet(a);

	Editor::EditorWindow w;
	w.show();

	return a.exec();
}
