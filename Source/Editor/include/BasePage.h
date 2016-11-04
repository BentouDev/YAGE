//
// Created by bentoo on 10/24/16.
//

#ifndef GAME_BASEPAGE_H
#define GAME_BASEPAGE_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>

namespace Editor
{
	class BasePage : public QWidget
	{
		Q_OBJECT

		QHBoxLayout* _toolbarLayout;

	public:
		BasePage(QWidget *parent = nullptr);
		void addButton(const char*, const char* = "");
	};
}

#endif //GAME_BASEPAGE_H
