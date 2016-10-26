//
// Created by bentoo on 10/24/16.
//
#include <QApplication>
#include <QFile>
#include "EditorWindow.h"

int main(int argc, char** args)
{
	QApplication a(argc, args);

	QFile stylesheet( ":/resources/EditorWindow.css" );
	if(stylesheet.open(QFile::ReadOnly))
	{
		QString styleSheet = stylesheet.readAll();
		a.setStyleSheet( styleSheet );
	}

	Editor::EditorWindow w;
	w.show();

	return a.exec();
}
