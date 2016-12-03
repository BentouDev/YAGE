//
// Created by bentoo on 10/24/16.
//
#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include "EditorWindow.h"

int main(int argc, char** args)
{
	QApplication a(argc, args);

	/*QFile stylesheet( ":/resources/EditorWindow.css" );
	if(stylesheet.open(QFile::ReadOnly))
	{
		QString styleSheet = stylesheet.readAll();
		a.setStyleSheet( styleSheet );
	}*/

	a.setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	//darkPalette.setColor(QPalette::Window, QColor(53,53,53));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(25,25,25));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53,53,53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	a.setPalette(darkPalette);

	a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

	Editor::EditorWindow w;
	w.show();

	return a.exec();
}
