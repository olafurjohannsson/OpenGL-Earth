#ifndef MAP_MAP_H
#define MAP_MAP_H

#include <QQuickItem>
#include <iostream>
#include "Renderer.h"
#include <memory>
#include "DataLoader.h"
#include <glm/glm.hpp>

namespace Map
{
    class Map : public QQuickItem
    {
        Q_OBJECT
        QML_ELEMENT

    public:
        Map(QQuickItem *parent = nullptr);

        glm::vec2 m_delta;
        glm::vec2 m_center = glm::vec2(1280, 960);
        bool m_holdingDownButton = false;

        void mouseMoveEvent(QMouseEvent *event) override
        {
            if (m_holdingDownButton)
            {
                glm::vec2 currentPosition = glm::vec2(event->position().x(), event->position().y());
                std::cout << "holding down btn: " << event->position().x() << ":" << event->position().y() << "\n";

                auto delta = m_center - currentPosition;

                if ((m_delta - delta).x > 0)
                {
                    m_renderer->panRight();
                }
                else if ((m_delta - delta).x < 0)
                {
                    m_renderer->panLeft();
                }
                else if ((m_delta - delta).y > 0)
                {
                    m_renderer->panUp();
                }
                else if ((m_delta - delta).y < 0)
                {
                    m_renderer->panDown();
                }

                std::cout << (m_delta - delta).x << ":" << (m_delta - delta).y << "\n";

                m_delta = delta;
                std::cout << "delta: " << delta.x << ":" << delta.y << "\n";
                // m_renderer->pan();
            }
        }

        void mousePressEvent(QMouseEvent *event) override
        {
            std::cout << event->button() << "-" << Qt::LeftButton << "\n";
            if (event->button() == Qt::LeftButton)
            {
                m_holdingDownButton = true;
            }
        }
        void mouseReleaseEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton)
            {
                std::cout << "leftBtn release";
                m_holdingDownButton = false;
            }
        }

        void wheelEvent(QWheelEvent *event) override
        {
            auto zoomIn = event->angleDelta().y() > 0, zoomOut = event->angleDelta().y() < 0;

            if (zoomIn)
            {
                m_renderer->zoomIn(event->position().x(), event->position().y());
            }
            else if (zoomOut)
            {
                m_renderer->zoomOut(event->position().x(), event->position().y());
            }
        }

    public slots:
        void sync();
        void cleanup();

        void render();

    private slots:
        void handleWindowChanged(QQuickWindow *window);

    protected:
        Renderer *m_renderer{nullptr};
        std::unique_ptr<DataLoader> m_dataLoader{nullptr};
    };
}

#endif // MAP_MAP_H