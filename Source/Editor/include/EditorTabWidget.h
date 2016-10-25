//
// Created by Bentoo on 2016-10-25.
//

#ifndef GAME_EDITORTABWIDGET_H
#define GAME_EDITORTABWIDGET_H

#include <QTabWidget>

namespace Editor
{
	class EditorTabWidget : public QTabWidget
	{
		Q_OBJECT

	public:
		EditorTabWidget(QWidget* parent = nullptr);
	};
}

#endif //GAME_EDITORTABWIDGET_H
