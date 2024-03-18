#include "Map.h"

namespace Map
{

    Map::Map()
    {
        std::cout << "Map Created\n";
        connect(this, &QQuickItem::windowChanged, this, &Map::handleWindowChanged);
    }

    void Map::sync()
    {
        if (!m_renderer)
        {
            m_renderer = new Renderer(this);
            // connect(window(), &QQuickWindow::beforeRendering, m_renderer, &Renderer::render);
        }

        m_renderer->setViewportSize(QSize(width(), height()));
        m_renderer->setClearColor(QColor(Qt::black));
        m_renderer->setRotation(0);
        m_renderer->setZoom(1);
        m_renderer->setCenter(QPointF(0, 0));
        m_renderer->render();
    }

    void Map::handleWindowChanged(QQuickWindow *window)
    {
        if (window)
        {
            connect(window, &QQuickWindow::beforeRendering, this, &Map::sync);
            connect(window, &QQuickWindow::beforeSynchronizing, this, &Map::sync);
            window->setColor(QColor(Qt::black));
        }
    }

}
