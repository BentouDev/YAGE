//
// Created by bentoo on 10/24/16.
//
#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include "EditorWindow.h"

void initStyleSheet(QApplication& a)
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

void setDarkStyle(QApplication& a, QPalette& darkPalette)
{
	a.setStyle(QStyleFactory::create("Fusion"));

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

	a.setPalette(darkPalette);

	a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

int main(int argc, char** args)
{
	QApplication a(argc, args);

	QPalette darkPalette;
	setDarkStyle(a, darkPalette);
	initStyleSheet(a);

	Editor::EditorWindow w;
	Editor::Editor instance;
	w.show();

	return a.exec();
}
