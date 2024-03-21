
#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Map
{
    class Renderer : public QObject, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        ~Renderer();

        void setVertices(const std::vector<glm::vec3> &vertices);

        void setViewportSize(const QSize &size);
        void setWindow(QQuickWindow *window);

    public slots:
        void init();
        void paint();

    protected:
        QSize m_viewportSize;
        qreal m_t = 0.0;
        QOpenGLShaderProgram *m_program = nullptr;
        QQuickWindow *m_window = nullptr;
        std::vector<glm::vec3> m_vertices;
    };
}

#endif // MAP_RENDERER_H