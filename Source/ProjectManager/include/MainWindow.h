#ifndef PROJECT_MANAGER_MAIN_WINDOW_H
#define PROJECT_MANAGER_MAIN_WINDOW_H

#include <QMainWindow>
#include <QFileSelector>
#include <QtQuick/QQuickView>

class MainWindow : public QMainWindow
{
public:
     MainWindow();
    ~MainWindow();

    void ReloadUI();

private:
    void resizeEvent(QResizeEvent* event) override;
    void delayReloadUI();

    QFileSelector* _FileSelector;
    QQuickView* _View;
    QWidget* _ViewWidget;
};

#endif //  PROJECT_MANAGER_MAIN_WINDOW_H