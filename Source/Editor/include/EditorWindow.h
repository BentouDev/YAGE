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

namespace ads
{
    class CDockManager;
}

namespace Editor
{
    class EditorApp;
    class EditorTabWidget;

    class EditorWindow : public QMainWindow
    {
    Q_OBJECT

    protected:
        EditorTabWidget* _tabWidget;

        void addTabPage(const char* name = "New Scene.scn*");

    public:
        explicit EditorWindow(EditorApp* editor, QWidget *parent = nullptr);
        ~EditorWindow();

    private slots:
        void on_actionExit_triggered();
        void onAddButtonClick();
        void onCloseTabButtonClick(int);

    private:
        EditorApp* _editor;
        Ui::EditorWindow* _ui;
        ads::CDockManager* m_DockManager;
    };
}

#endif //GAME_EDITORWINDOW_H
