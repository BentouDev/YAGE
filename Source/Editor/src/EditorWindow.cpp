//
// Created by bentoo on 10/24/16.
//
#include <Utils/SafeDelete.h>
#include <QMouseEvent>
#include <QDockWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>

#include "EditorWindow.h"
#include "EditorTabWidget.h"
#include "BasePage.h"

#include "ui_EditorWindow.h"

#ifdef Q_OS_WIN
#include <QtWin>
#endif

namespace Editor
{
	EditorWindow::EditorWindow(QWidget *parent) :
			QMainWindow(parent),
			_ui(new Ui::EditorWindow)
	{
#ifdef Q_OS_WIN
		// Transparent background for windows
		this->setAttribute(Qt::WA_TranslucentBackground);
	    QtWin::enableBlurBehindWindow(this);
    	QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
#endif
		_ui->setupUi(this);

		QLayout* layout = _ui->centralWidget->layout();

		_tabWidget = new EditorTabWidget(_ui->centralWidget);

		layout->addWidget(_tabWidget);
		layout->setMargin(0);
		layout->setSpacing(0);

		addTabPage();

		connect(_tabWidget, SIGNAL(plusButtonClicked()), this, SLOT(onAddButtonClick()));
		connect(_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTabButtonClick(int)));
	}

	void EditorWindow::onAddButtonClick()
	{
		addTabPage();
	}

	void EditorWindow::onCloseTabButtonClick(int index)
	{
		_tabWidget->removeTab(index);

		if(_tabWidget->count() == 0)
		{
			close();
		}
	}

	void EditorWindow::addTabPage(const char* name)
	{
		BasePage* page = new BasePage(this);
		_tabWidget->addTab(page, name);
	}

	EditorWindow::~EditorWindow()
	{
		Memory::SafeDelete(_ui);
	}

	void EditorWindow::on_actionExit_triggered()
	{
		close();
	}
}
