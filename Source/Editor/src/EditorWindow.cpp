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
		// Transparent background for windows
#ifdef Q_OS_WIN
		this->setAttribute(Qt::WA_TranslucentBackground);
	    QtWin::enableBlurBehindWindow(this);
    	QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
#endif

		QToolButton* addButton = new QToolButton();
		addButton->setText("+");
		addButton->setFixedSize(24,24);
		addButton->setContentsMargins(4,4,4,4);

		_ui->setupUi(this);

		QLayout* layout = _ui->centralWidget->layout();

		_tabWidget = new EditorTabWidget(_ui->centralWidget);
		_tabWidget->setTabsClosable(true);

		layout->addWidget(_tabWidget);
		layout->setMargin(0);
		layout->setSpacing(0);

		addTabPage("YAGE");

		_tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->hide();
		_tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0,0);
		_tabWidget->setCornerWidget(addButton);

		connect(addButton, SIGNAL (released()), this, SLOT (onAddButtonClick()));
		connect(_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTabButtonClick(int)));
	}

	void EditorWindow::onAddButtonClick()
	{
		addTabPage();
	}

	void EditorWindow::onCloseTabButtonClick(int index)
	{
		_tabWidget->removeTab(index);
	}

	void EditorWindow::addTabPage(const char* name)
	{
		BasePage* page = new BasePage(this);
		_tabWidget->addTab(page, name);
		_tabWidget->tabBar()->tabButton(_tabWidget->count() - 1, QTabBar::RightSide)->resize(10,10);
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
