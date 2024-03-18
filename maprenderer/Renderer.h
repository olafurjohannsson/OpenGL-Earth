
#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

namespace Map
{
    class Renderer : public QObject, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        Renderer(QQuickItem *parent = nullptr);
        ~Renderer();

        void setViewportSize(const QSize &size);
        void setClearColor(const QColor &color);
        void setRotation(float angle);
        void setZoom(float zoom);
        void setCenter(const QPointF &center);

        void render();

    public slots:
        void init();
        void paint();

    protected:
        QOpenGLShaderProgram *m_program = nullptr;
        QSize m_viewportSize;
        QColor m_clearColor;
        float m_rotation;
        float m_zoom;
        QPointF m_center;
    };
}

#endif // MAP_RENDERER_H