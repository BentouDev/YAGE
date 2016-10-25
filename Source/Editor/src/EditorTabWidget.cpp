//
// Created by Bentoo on 2016-10-25.
//

#include "EditorTabWidget.h"
#include "EditorTabBar.h"

namespace Editor
{
	EditorTabWidget::EditorTabWidget(QWidget* parent)
		: QTabWidget(parent)
	{
		setTabBar(new EditorTabBar(this));
	}
}
