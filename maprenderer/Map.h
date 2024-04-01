#ifndef MAP_MAP_H
#define MAP_MAP_H

#include <QQuickItem>
#include <iostream>
#include "Renderer.h"
#include <memory>
#include "GeographicLoader.h"
#include "Projection.h"
#include <glm/glm.hpp>

namespace Map
{
    class Map : public QQuickItem
    {
        Q_OBJECT
        QML_ELEMENT
        Q_PROPERTY( QString cursorCoordinate READ cursorCoordinate NOTIFY cursorCoordinateChanged )
        Q_PROPERTY( float zoom READ zoom NOTIFY zoomChanged )
        Q_PROPERTY( float centerX READ centerX NOTIFY centerChanged )
        Q_PROPERTY( float centerY READ centerY NOTIFY centerChanged )
    public:
        Map(QQuickItem *parent = nullptr);

        float zoom() const;
        float centerX() const;
        float centerY() const;
        
        QString cursorCoordinate() const;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void hoverMoveEvent(QHoverEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;

    signals:
        void cursorCoordinateChanged();
        void zoomChanged();
        void centerChanged();

    public slots:
        void sync();
        void cleanup();



    private slots:
        void handleWindowChanged(QQuickWindow *window);

    protected:
        // We keep the renderer as a raw pointer because qt is responsible for deleting it
        Renderer *m_renderer{nullptr};
        std::unique_ptr<GeographicLoader> m_geographicLoader{nullptr};
        std::unique_ptr<Projection> m_projection{nullptr};

        glm::vec2 m_previousPosition;
        
        bool m_holdingDownButton = false;
        glm::vec2 m_cursorPosition;

        double m_defaultLongitude = -22.0;
        double m_defaultLatitude = 64.0;
    };
}

#endif // MAP_MAP_H