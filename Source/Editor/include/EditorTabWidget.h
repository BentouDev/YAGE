//
// Created by Bentoo on 2016-10-25.
//

#ifndef GAME_EDITORTABWIDGET_H
#define GAME_EDITORTABWIDGET_H

#include <QTabWidget>
#include <QToolButton>

namespace Editor
{
	class EditorTabBar;

	class EditorTabWidget : public QTabWidget
	{
		Q_OBJECT

	protected:
		void placeMenuButton();
		virtual void resizeEvent(QResizeEvent *event) override;
		virtual bool event(QEvent* event) override;

		EditorTabBar* _editorTabBar;
		QToolButton* _mainMenuButton;

	protected slots:
		void emitPlusButtonClicked();
		void emitMainMenuButtonClicked();

	signals:
		void plusButtonClicked();
		void mainMenuButtonClicked();

	public:
		explicit EditorTabWidget(QWidget* parent = nullptr);
	};
}

#endif //GAME_EDITORTABWIDGET_H
