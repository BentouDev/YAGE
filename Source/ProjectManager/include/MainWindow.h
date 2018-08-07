#ifndef PROJECT_MANAGER_MAIN_WINDOW_H
#define PROJECT_MANAGER_MAIN_WINDOW_H

#include <QMainWindow>

class Backend;
class QmlFrame;

class MainWindow : public QMainWindow
{
public:
     MainWindow();
    ~MainWindow();

    void RegisterBackend(Backend& backend);

private:
    QmlFrame* _MainQml;
};

#endif //  PROJECT_MANAGER_MAIN_WINDOW_H