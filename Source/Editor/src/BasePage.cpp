//
// Created by bentoo on 10/24/16.
//

#include "BasePage.h"
#include <QLayout>
#include <QToolBar>
#include <QToolButton>

namespace Editor
{
	BasePage::BasePage(QWidget *parent)
		: QWidget(parent)
	{
		/*QToolBar* toolbar = new QToolBar(this);

		QWidget* segmentedButton = new QWidget(this);
		QToolButton* left = new QToolButton();
		QToolButton* right = new QToolButton();

		segmentedButton->setLayout(new QHBoxLayout());
		segmentedButton->layout()->setSpacing(0);
		segmentedButton->layout()->setMargin(0);

		int l = 0, r = 0, t = 0, b = 0;
		left->getContentsMargins(&l, &t, &r, &b);
		left->setContentsMargins(l - 4, t, r - 4, b);

		right->getContentsMargins(&l, &t, &r, &b);
		right->setContentsMargins(l - 4, t, r - 4, b);

		left->setFixedSize(28,28);
		right->setFixedSize(28,28);

		segmentedButton->layout()->addWidget(left);
		segmentedButton->layout()->addWidget(right);

		toolbar->addWidget(segmentedButton);*/
	}
}