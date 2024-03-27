#include "Map.h"
#include <iostream>


namespace Map
{

    Map::Map( QQuickItem *parent ) : QQuickItem( parent )
    {
        m_geographicLoader = std::make_unique< GeographicLoader >("../data/large.json");
        
        connect(this, &QQuickItem::windowChanged, this, &Map::handleWindowChanged);

        setAcceptedMouseButtons(Qt::LeftButton);
    }

    void Map::cleanup()
    {
        if (m_renderer)
        {
            delete m_renderer;
            m_renderer = nullptr;
        }
    }

    void Map::sync()
    {
        if (nullptr == m_renderer)
        {
            m_renderer = new Renderer;
            
            m_projection = std::make_unique<Projection>(window()->size().width(), window()->size().height());
            m_projection->setProjectionType(Projection::ProjectionType::Mercator);

            m_geographicLoader->project(*m_projection);
            
            m_renderer->setPolygons(m_geographicLoader->polygons());
            
            connect(window(), &QQuickWindow::beforeRendering, m_renderer, &Renderer::init, Qt::DirectConnection);
            connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &Renderer::paint, Qt::DirectConnection);
        }

        m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
        m_renderer->setWindow(window());

    }

    void Map::render()
    {
        if (m_renderer)
        {
            m_renderer->paint();
        }
    }

    void Map::handleWindowChanged(QQuickWindow *window)
    {
        if (window)
        {
            connect(window, &QQuickWindow::beforeSynchronizing, this, &Map::sync, Qt::DirectConnection);
            connect(window, &QQuickWindow::sceneGraphInvalidated, this, &Map::cleanup, Qt::DirectConnection);
            window->setColor(QColor(Qt::black));
        }
    }

}
