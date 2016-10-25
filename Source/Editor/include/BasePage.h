//
// Created by bentoo on 10/24/16.
//

#ifndef GAME_BASEPAGE_H
#define GAME_BASEPAGE_H

#include <QWidget>

namespace Editor
{
	class BasePage : public QWidget
	{
		Q_OBJECT

	public:
		BasePage(QWidget *parent = nullptr);
	};
}

#endif //GAME_BASEPAGE_H
