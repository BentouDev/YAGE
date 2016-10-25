//
// Created by Bentoo on 2016-10-25.
//

#ifndef GAME_EDITORTABBAR_H
#define GAME_EDITORTABBAR_H

#include <QTabBar>

namespace Editor
{
	class EditorTabBar : public QTabBar
	{
		Q_OBJECT

	protected:
		void mousePressEvent(QMouseEvent* event);

	public:
		explicit EditorTabBar(QWidget* parent = nullptr);
		void moveTab(int from, int to);
	};
}

#endif //GAME_EDITORTABBAR_H
