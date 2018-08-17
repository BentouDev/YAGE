#ifndef PROJECT_MANAGER_MAIN_WINDOW_H
#define PROJECT_MANAGER_MAIN_WINDOW_H

#include <QMainWindow>

class Backend;
class QPixmap;
class QmlFrame;
class QSplashScreen;

class MainWindow : public QMainWindow
{
public:
     MainWindow();
    ~MainWindow();

    void RegisterBackend(Backend& backend);

private:
    QmlFrame*      _MainQml;
    QSplashScreen* _Splash;
    QPixmap*       _SplashImage;
};

#endif //  PROJECT_MANAGER_MAIN_WINDOW_H