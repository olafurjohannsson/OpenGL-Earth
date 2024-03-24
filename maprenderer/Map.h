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

        glm::vec2 m_previousPosition;
        glm::vec2 m_center = glm::vec2(1280, 960);
        bool m_holdingDownButton = false;

        void mouseMoveEvent(QMouseEvent *event) override
        {
            if (m_holdingDownButton)
            {
                glm::vec2 currentPosition = glm::vec2(event->position().x(), window()->height() - event->position().y());

                // Calculate pan direction based on the difference between the current and previous cursor position
                glm::vec2 offset = currentPosition - m_previousPosition;

                m_previousPosition = currentPosition;

                // Pass pan direction to a single pan method
                m_renderer->pan(offset);
            }
        }

        void mousePressEvent(QMouseEvent *event) override
        {
            std::cout << event->button() << "-" << Qt::LeftButton << "\n";
            if (event->button() == Qt::LeftButton)
            {
                m_holdingDownButton = true;
                m_previousPosition = glm::vec2(event->position().x(), window()->height() - event->position().y());
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
            auto zoomOut = event->angleDelta().y() > 0, zoomIn = event->angleDelta().y() < 0;

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