#ifndef YAGE_EDITOR_BASEPAGE_H
#define YAGE_EDITOR_BASEPAGE_H

#include "BasePage.h"
#include <QSurface>

namespace Editor
{
    class SceneView : public BasePage
    {
    public:

    protected:
        QSurface* _surface;
    };
}

#endif