
#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include "lib/Coordinate.h"
#include "lib/Polygon.h"

namespace Map
{
    class Renderer : public QObject, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:

        ~Renderer();
        void logMouse(float x, float y);
        void setPolygons(const std::vector<Polygon> &polygons);
        void setViewportSize(const QSize &size);
        void setWindow(QQuickWindow *window);

        void updateMatrix(const glm::mat4 &matrix);

        void pan(const glm::vec2 &direction);
        void zoomIn(int x, int y);
        void zoomOut(int x, int y);
        void setZoom(float zoom);
        void test(int x, int y);

    public slots:
        void init();
        void paint();

    protected:
        QSize m_viewportSize;
        QOpenGLShaderProgram *m_program = nullptr;
        QQuickWindow *m_window = nullptr;
        
        std::vector<Polygon> m_polygons;
        float m_zoomFactor = 1.0f;
        
        glm::mat4 m_matrix = glm::mat4(1.0f);

        float m_centerX;
        float m_centerY;
        double m_centerLongitude;
        double m_centerLatitude;
    };
}

#endif // MAP_RENDERER_H