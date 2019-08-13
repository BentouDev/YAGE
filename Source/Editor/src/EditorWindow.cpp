//
// Created by bentoo on 10/24/16.
//
#include <Utils/SafeDelete.h>
#include <QMouseEvent>
#include <QDockWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QTabBar>
#include <QLabel>
#include <QToolButton>

#include "EditorWindow.h"
#include "EditorTabWidget.h"
#include "Pages/BasePage.h"
#include "Pages/SceneView.h"

#include "ui_EditorWindow.h"

#ifdef Q_OS_WIN
#include <QtWin>
#endif

#include <DockManager.h>

#include "IconTitleWidget.h"

namespace Editor
{
    static int CONTENT_COUNT = 0;

    static ads::CDockWidget* createLongTextLabelSC()
    {
        QWidget* w = new QWidget();
        QBoxLayout* bl = new QBoxLayout(QBoxLayout::TopToBottom);
        w->setLayout(bl);

        QLabel* l = new QLabel();
        l->setWordWrap(true);
        l->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        l->setText(QString("Lorem Ipsum ist ein einfacher Demo-Text für die Print- und Schriftindustrie. Lorem Ipsum ist in der Industrie bereits der Standard Demo-Text seit 1500, als ein unbekannter Schriftsteller eine Hand voll Wörter nahm und diese durcheinander warf um ein Musterbuch zu erstellen. Es hat nicht nur 5 Jahrhunderte überlebt, sondern auch in Spruch in die elektronische Schriftbearbeitung geschafft (bemerke, nahezu unverändert). Bekannt wurde es 1960, mit dem erscheinen von Letrase, welches Passagen von Lorem Ipsum enhielt, so wie Desktop Software wie Aldus PageMaker - ebenfalls mit Lorem Ipsum."));
        bl->addWidget(l);

        ads::CDockWidget* DockWidget = new ads::CDockWidget("Label 1");
        DockWidget->setWidget(w);

        return DockWidget;
    }

    static ads::CDockWidget* createBasePage(EditorApp* editor)
    {
        QWidget* w = new QWidget();
        QBoxLayout* bl = new QBoxLayout(QBoxLayout::TopToBottom);
        w->setLayout(bl);

        BasePage* p = new SceneView(editor);
        bl->addWidget(p);

        ads::CDockWidget* DockWidget = new ads::CDockWidget("Label 1");
        DockWidget->setWidget(p);

        return DockWidget;
    }

    EditorWindow::EditorWindow(EditorApp* editor, QWidget* parent)
        : QMainWindow(parent)
        , _editor(editor)
        , _ui(new Ui::EditorWindow)
    {
#ifdef Q_OS_WIN
        // Transparent background for windows
        //this->setAttribute(Qt::WA_TranslucentBackground);
        //QtWin::enableBlurBehindWindow(this);
        //QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
#endif

        _ui->setupUi(this);

        m_DockManager = new ads::CDockManager(this);

        m_DockManager->addDockWidget(ads::TopDockWidgetArea, createLongTextLabelSC());
        m_DockManager->addDockWidget(ads::TopDockWidgetArea, createLongTextLabelSC());
        m_DockManager->addDockWidget(ads::TopDockWidgetArea, createLongTextLabelSC());
        m_DockManager->addDockWidget(ads::TopDockWidgetArea, createBasePage(_editor));
        m_DockManager->addDockWidget(ads::TopDockWidgetArea, createBasePage(_editor));
    }

    void EditorWindow::onAddButtonClick()
    {
        addTabPage();
    }

    void EditorWindow::onCloseTabButtonClick(int index)
    {
        _tabWidget->removeTab(index);

        if (_tabWidget->count() == 0)
        {
            close();
        }
    }

    void EditorWindow::addTabPage(const char* name)
    {
        BasePage* page = new BasePage(_editor, this);
        _tabWidget->addTab(page, name);
    }

    EditorWindow::~EditorWindow()
    {
        // Memory::SafeDelete(_ui);
    }

    void EditorWindow::on_actionExit_triggered()
    {
        close();
    }
}
