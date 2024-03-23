#include "Map.h"
#include <iostream>
#include "DataLoader.h"

namespace Map
{

    Map::Map( QQuickItem *parent ) : QQuickItem( parent )
    {
        std::cout << "Map Created width: " << width() << " height: " << height() << "\n";
        m_dataLoader = std::make_unique< DataLoader >();
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
            std::cout << "Creating Renderer width: " << width() << " height: " << height() << "\n";
            m_renderer = new Renderer;
            m_renderer->setVertices(m_dataLoader->getVertices());
            // m_renderer->init();
            std::cout << window()->screen()->size().width() << " " << window()->screen()->size().height() << "\n";
            connect(window(), &QQuickWindow::beforeRendering, m_renderer, &Renderer::init, Qt::DirectConnection);
            connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &Renderer::paint, Qt::DirectConnection);
        }

        m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
        m_renderer->setWindow(window());

    }

    void Map::render()
    {
        std::cout << "Render\n";
        if (m_renderer)
        {
            m_renderer->paint();
        }
    }

    void Map::handleWindowChanged(QQuickWindow *window)
    {
        if (window)
        {
            std::cout << "Window Changed\n";
            connect(window, &QQuickWindow::beforeSynchronizing, this, &Map::sync, Qt::DirectConnection);
            connect(window, &QQuickWindow::sceneGraphInvalidated, this, &Map::cleanup, Qt::DirectConnection);
            window->setColor(QColor(Qt::black));
        }
    }

}
