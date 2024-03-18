#ifndef MAP_MAP_H
#define MAP_MAP_H

#include <QQuickItem>
#include <iostream>
#include "Renderer.h"

namespace Map
{
    class Map : public QQuickItem
    {
        Q_OBJECT
        QML_ELEMENT

    public:
        Map();

    public slots:
        void sync();

    private slots:
        void handleWindowChanged(QQuickWindow *window);

    protected:
        Renderer *m_renderer { nullptr };
    };
}

#endif // MAP_MAP_H