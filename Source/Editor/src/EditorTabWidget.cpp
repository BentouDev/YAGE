//
// Created by Bentoo on 2016-10-25.
//

#include <QtCore/QEvent>
#include "EditorTabWidget.h"
#include "EditorTabBar.h"

namespace Editor
{
	EditorTabWidget::EditorTabWidget(QWidget* parent)
		: QTabWidget(parent)
	{

		_editorTabBar = new EditorTabBar(this);
		setTabBar(_editorTabBar);
		setTabsClosable(true);
		setElideMode( Qt::ElideNone );
		setUsesScrollButtons(true);
		setMovable(true);

		_mainMenuButton = new QToolButton(this);
		_mainMenuButton->setObjectName("mainMenuButton");
		_mainMenuButton->setText("YAGE");
		_mainMenuButton->setFixedHeight(_editorTabBar->height());
		setCornerWidget(_mainMenuButton, Qt::TopLeftCorner);

		connect(_editorTabBar, SIGNAL(plusButtonClicked()), this, SLOT(emitPlusButtonClicked()));
		connect(_mainMenuButton, SIGNAL(released()), this, SLOT(emitMainMenuButtonClicked()));
	}

	bool EditorTabWidget::event(QEvent* event)
	{
		if(event->type() == QEvent::LayoutRequest)
		{
			QTabWidget::event(event);
			placeMenuButton();
			return true;
		}

		return QTabWidget::event(event);
	}

	void EditorTabWidget::resizeEvent(QResizeEvent *event)
	{
		QTabWidget::resizeEvent(event);
		placeMenuButton();
	}

	void EditorTabWidget::placeMenuButton()
	{
		_mainMenuButton->move(0,0);
	}

	void EditorTabWidget::emitPlusButtonClicked()
	{
		emit plusButtonClicked();
	}

	void EditorTabWidget::emitMainMenuButtonClicked()
	{
		emit mainMenuButtonClicked();
	}
}
