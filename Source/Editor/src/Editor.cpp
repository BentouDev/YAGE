#include "Editor.h"
#include <Utils/MemorySizes.h>
#include <Utils/SafeDelete.h>

#include <Platform/Logger.h>
#include <Platform/FileLoggerOutput.h>

#include <Core/Engine.h>

#include <QApplication>
#include <QStyleFactory>
#include <QFile>

#include "EditorWindow.h"

#include <ctime>

namespace Editor
{
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

    EditorApp::EditorApp()
    {

    }

    int EditorApp::Exec(int argc, char** args)
    {
        QApplication a(argc, args);

	    QPalette darkPalette;
	    setDarkStyle(a, darkPalette);
	    initStyleSheet(a);

        // show splash

		time_t rawtime;
		time(&rawtime);
		struct tm* start_date = localtime(&rawtime);

		Core::Logger::get().createOutput<Core::FileLoggerOutput>
		(
			fmt::format("Log/Log-{}.{}.{}-[{}.{}.{}].txt",
			start_date->tm_year, start_date->tm_mon, start_date->tm_mday,
			start_date->tm_hour, start_date->tm_min, start_date->tm_sec
		).c_str());

        _Engine = new Core::Engine("Editor", Memory::MB(20));
		_Engine->LoadConfig("Config/EditorConfig.json");

        // init subsystems

		if (!_Engine->Initialize())
		{
			return -1;
		}

        _MainWindow = new EditorWindow(this);
		_MainWindow->show();

        return a.exec();
    }

    EditorApp::~EditorApp()
    {
        Memory::SafeDelete(_Engine);
        
        delete _MainWindow;
    }
}
