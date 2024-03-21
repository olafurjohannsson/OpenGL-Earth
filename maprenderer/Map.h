#ifndef MAP_MAP_H
#define MAP_MAP_H

#include <QQuickItem>
#include <iostream>
#include "Renderer.h"
#include <memory>
#include "DataLoader.h"

namespace Map
{
    class Map : public QQuickItem
    {
        Q_OBJECT
        QML_ELEMENT

    public:
        Map( QQuickItem *parent = nullptr );

    public slots:
        void sync();
        void cleanup();

    private slots:
        void handleWindowChanged(QQuickWindow *window);

    protected:
        Renderer *m_renderer{nullptr};
        std::unique_ptr< DataLoader > m_dataLoader{nullptr};
    };
}

#endif // MAP_MAP_H