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
            m_projection->setProjectionType(Projection::ProjectionType::Stereographic);
            m_projection->setCenterLongitudeLatitude( 64.0, -20.0 );
            m_geographicLoader->project(*m_projection);
            m_projection->onZoom(1);
            // m_renderer->setZoom(m_projection->calculateZoomFactor());
            m_renderer->updateMatrix(m_projection->matrix());
            m_renderer->setPolygons(m_geographicLoader->polygons());
            
            connect(window(), &QQuickWindow::beforeRendering, m_renderer, &Renderer::init, Qt::DirectConnection);
            connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &Renderer::paint, Qt::DirectConnection);
        }

        m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
        m_renderer->setWindow(window());

    }
    void Map::mouseMoveEvent(QMouseEvent *event)
    {
        if (m_holdingDownButton)
        {
            glm::vec2 currentPosition = glm::vec2(event->position().x(), window()->height() - event->position().y());

            // Calculate pan direction based on the difference between the current and previous cursor position
            glm::vec2 offset = currentPosition - m_previousPosition;

            m_previousPosition = currentPosition;

            // Pass pan direction to a single pan method
            m_renderer->pan(offset);
            
            m_projection->setCenter(offset.x, offset.y);
            m_renderer->updateMatrix(m_projection->matrix());
        }
        
        // std::cout << "x: " << event->position().x() << " y: " << event->position().y() << "\n";
        // auto x = event->position().x();
        // auto y = event->position().y();
        // std::optional<glm::vec2> unprojected = m_projection->unproject(x, y);
        // if (unprojected.has_value())
        // {
        //     m_renderer->test(unprojected->x, unprojected->y);
        // }
    }

    void Map::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
        {
            m_holdingDownButton = true;
            m_previousPosition = glm::vec2(event->position().x(), window()->height() - event->position().y());
        }
    }
    void Map::mouseReleaseEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
        {
            m_holdingDownButton = false;
        }
    }

    void Map::wheelEvent(QWheelEvent *event)
    {
        auto zoomOut = event->angleDelta().y() > 0, zoomIn = event->angleDelta().y() < 0;
        m_projection->onZoom(zoomIn ? -1 : 1);
        // m_renderer->setZoom(m_projection->calculateZoomFactor());
        
        m_renderer->updateMatrix(m_projection->matrix());

        // 

        // if (zoomIn)
        // {
        //     m_renderer->zoomIn(event->position().x(), event->position().y());
        // }
        // else if (zoomOut)
        // {
        //     m_renderer->zoomOut(event->position().x(), event->position().y());
        // }
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
