//
// Created by Bentoo on 2016-10-25.
//

#include "EditorTabBar.h"
#include <QMouseEvent>

namespace Editor
{
	EditorTabBar::EditorTabBar(QWidget* parent)
		: QTabBar(parent)
	{

	}

	void EditorTabBar::mousePressEvent(QMouseEvent *event)
	{
		// all tabs movable except first
		setMovable(tabAt(event->pos()) != 0);

		QTabBar::mousePressEvent(event);
	}

	void EditorTabBar::moveTab(int from, int to)
	{
		if(to == 0)
			return;

		QTabBar::moveTab(from, to);
	}
}
