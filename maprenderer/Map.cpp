#include "Map.h"
#include <iostream>


namespace Map
{

    Map::Map( QQuickItem *parent ) : QQuickItem( parent )
    {
        m_projection = std::make_unique<Projection>(Projection::ProjectionType::Orthographic);
        m_geographicLoader = std::make_unique< GeographicLoader >("../data/large.json");
        connect(this, &QQuickItem::windowChanged, this, &Map::handleWindowChanged);
        setAcceptHoverEvents(true);
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
            // Create projection
            m_projection->setScreenSize(window()->size().width(), window()->size().height());
            m_projection->setCenter(m_defaultLongitude, m_defaultLatitude);
            
            // create renderer, update uniforms and set polygons
            m_renderer = new Renderer;
            m_renderer->updateUniforms(
                    m_projection->getCenter(), 
                    m_projection->projectionMatrix(), 
                    m_projection->scaleMatrix(),
                    m_projection->rotationMatrix());
            m_geographicLoader->project(*m_projection);
            m_renderer->setPolygons(m_geographicLoader->polygons());
            connect(window(), &QQuickWindow::beforeRendering, m_renderer, &Renderer::inititalize, Qt::DirectConnection);
            connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &Renderer::render, Qt::DirectConnection);
            emit centerChanged();
        }

        m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
        m_renderer->setWindow(window());
        m_renderer->setProjectionType(m_projection->projectionType());
    }
    
    float Map::zoom() const
    {
        return m_projection->zoomFactor();
    }
    float Map::centerX() const
    {
        return m_projection->getCenter().x;
    }
    float Map::centerY() const
    {
        return m_projection->getCenter().y;
    }
    QString Map::cursorCoordinate() const
    {
        return "";
    }
    void Map::hoverMoveEvent(QHoverEvent *event)
    {
        m_cursorPosition = glm::vec2(event->position().x(), window()->height() - event->position().y());
        emit cursorCoordinateChanged();
    }

    void Map::setProjection(const QString &projectionTypeString)
    {
        if (projectionTypeString == "Orthographic")
        {
            m_projection->setProjectionType(Projection::ProjectionType::Orthographic);
        }
        else if (projectionTypeString == "Mercator")
        {
            m_projection->setProjectionType(Projection::ProjectionType::Mercator);
        }
        else if (projectionTypeString == "Linear")
        {
            m_projection->setProjectionType(Projection::ProjectionType::LinearTransformation);
        }
        else if (projectionTypeString == "Stereographic")
        {
            m_projection->setProjectionType(Projection::ProjectionType::Stereographic);
        }
        m_renderer->setProjectionType(m_projection->projectionType());
        m_renderer->updateUniforms(
                    m_projection->getCenter(), 
                    m_projection->projectionMatrix(), 
                    m_projection->scaleMatrix(),
                    m_projection->rotationMatrix());
    }

    void Map::mouseMoveEvent(QMouseEvent *event)
    {
        if (m_holdingDownButton)
        {
            glm::vec2 currentPosition = glm::vec2(event->position().x(), window()->height() - event->position().y());
            // Calculate pan direction based on the difference between the current and previous cursor position
            glm::vec2 offset = currentPosition - m_previousPosition;
            m_previousPosition = currentPosition;

            m_projection->setOffset(offset.x, offset.y);
            m_renderer->updateUniforms(
                    m_projection->getCenter(), 
                    m_projection->projectionMatrix(), 
                    m_projection->scaleMatrix(),
                    m_projection->rotationMatrix());
            
            emit zoomChanged();
            emit centerChanged();
        }
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
        m_projection->zoom(event->angleDelta().y(), glm::vec2(event->position().x(), event->position().y()));
        
        m_renderer->updateUniforms(
                    m_projection->getCenter(), 
                    m_projection->projectionMatrix(), 
                    m_projection->scaleMatrix(),
                    m_projection->rotationMatrix());
        emit zoomChanged();
        emit centerChanged();
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
