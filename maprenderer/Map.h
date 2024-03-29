#ifndef MAP_MAP_H
#define MAP_MAP_H

#include <QQuickItem>
#include <iostream>
#include "Renderer.h"
#include <memory>
#include "lib/GeographicLoader.h"
#include "lib/Projection.h"
#include <glm/glm.hpp>

namespace Map
{
    class Map : public QQuickItem
    {
        Q_OBJECT
        QML_ELEMENT

    public:
        Map(QQuickItem *parent = nullptr);

        

        void mouseMoveEvent(QMouseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;

    public slots:
        void sync();
        void cleanup();

        void render();

    private slots:
        void handleWindowChanged(QQuickWindow *window);

    protected:
        // We keep the renderer as a raw pointer because qt is responsible for deleting it
        Renderer *m_renderer{nullptr};
        std::unique_ptr<GeographicLoader> m_geographicLoader{nullptr};
        std::unique_ptr< Projection> m_projection{nullptr};

        glm::vec2 m_previousPosition;
        glm::vec2 m_center = glm::vec2(1280, 960);
        bool m_holdingDownButton = false;
    };
}

#endif // MAP_MAP_H