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
		plusButton = new QToolButton(this);
		plusButton->setIcon(QIcon::fromTheme("plus"));
		plusButton->setObjectName("newTabButton");
		plusButton->setFixedSize(28,20);
		plusButton->setParent(this);
		placePlusButton();

		connect(plusButton, SIGNAL(released()), this, SLOT(emitPlusButtonClicked()));
	}

	void EditorTabBar::emitPlusButtonClicked()
	{
		emit plusButtonClicked();
	}

	QToolButton* EditorTabBar::getPlusButton() const
	{
		return plusButton;
	}

	void EditorTabBar::placePlusButton()
	{
		int size = 0;
		for(int i = 0; i < count(); i++)
		{
			size += tabRect(i).width();
		}

		int offset = (int)((geometry().height() - plusButton->height()) * 0.5f);

		int h = geometry().top() + offset;
		int w = width();

		if(size > w)
		{
			plusButton->move(w - 54, h);
		}
		else
		{
			plusButton->move(size, h);
		}
	}

	void EditorTabBar::resizeEvent(QResizeEvent *event)
	{
		QTabBar::resizeEvent(event);
		placePlusButton();
	}

	void EditorTabBar::tabLayoutChange()
	{
		QTabBar::tabLayoutChange();
		placePlusButton();
	}

	QSize EditorTabBar::minimumSizeHint() const
	{
		QSize sizeHint = QTabBar::minimumSizeHint();
		int width = sizeHint.width();
		int height = sizeHint.height();
		return sizeHint;//QSize(width + plusButton->width(), height);
	}

	QSize EditorTabBar::sizeHint() const
	{
		QSize sizeHint = QTabBar::sizeHint();
		int width = sizeHint.width();
		int height = sizeHint.height();
		return QSize(width + plusButton->width(), height);
	}
}
