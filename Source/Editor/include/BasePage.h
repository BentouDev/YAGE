//
// Created by bentoo on 10/24/16.
//

#ifndef GAME_BASEPAGE_H
#define GAME_BASEPAGE_H

#include <QFrame>
#include <QtWidgets/QHBoxLayout>

namespace Editor
{
	class BasePage : public QFrame
	{
		Q_OBJECT

		QHBoxLayout* _toolbarLayout;

	public:
		BasePage(QWidget *parent = nullptr);
		void addButton(const char*, const char* = "");
	};
}

#endif //GAME_BASEPAGE_H
