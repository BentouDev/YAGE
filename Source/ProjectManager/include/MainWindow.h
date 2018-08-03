#ifndef PROJECT_MANAGER_MAIN_WINDOW_H
#define PROJECT_MANAGER_MAIN_WINDOW_H

#include <QMainWindow>
#include <QFileSelector>
#include <QtQuick/QQuickView>

class Backend;

class MainWindow : public QMainWindow
{
public:
     MainWindow();
    ~MainWindow();

    void ReloadUI();
    void RegisterBackend(Backend& backend);

private:
    void resizeEvent(QResizeEvent* event) override;

    QFileSelector* _FileSelector;
    QQuickView* _View;
    QWidget* _ViewWidget;
};

#endif //  PROJECT_MANAGER_MAIN_WINDOW_H