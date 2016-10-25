//
// Created by bentoo on 10/24/16.
//

#ifndef GAME_EDITORWINDOW_H
#define GAME_EDITORWINDOW_H

#include <QMainWindow>

namespace Ui
{
	class EditorWindow;
}

namespace Editor
{
	class EditorWindow : public QMainWindow
	{
	Q_OBJECT

	protected:
		QTabWidget* _tabWidget;
		QPoint _dragPosition;

		void addTabPage(const char* name = "New Page");

	public:
		explicit EditorWindow(QWidget *parent = nullptr);
		~EditorWindow();

	private slots:
		void on_actionExit_triggered();
		void onAddButtonClick();
		void onCloseTabButtonClick(int);

	private:
		Ui::EditorWindow *_ui;
	};
}

#endif //GAME_EDITORWINDOW_H
