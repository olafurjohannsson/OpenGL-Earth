
#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Coordinate.h"
#include "Polygon.h"
#include "Projection.h"
#include "OpenGL.h"

namespace Map
{
    

    class Renderer : public QObject
    {
        Q_OBJECT
    public:
        Renderer() = default;
        // disable copy
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        ~Renderer();
        
        void setPolygons(const std::vector<Polygon> &polygons);
        void setViewportSize(const QSize &size);
        void setWindow(QQuickWindow *window);
        void setProjectionType(Projection::ProjectionType type);
        void updateUniforms(const glm::vec2& center, const glm::mat4 &projectionMatrix, const glm::mat4& scaleMatrix, const glm::mat4& rotationMatrix);


    public slots:
        void inititalize();
        void render();

    protected:
        QSize m_viewportSize;
        QOpenGLShaderProgram *m_program = nullptr;
        QQuickWindow *m_window = nullptr;
        
        std::vector<Polygon> m_polygons;

        Projection::ProjectionType m_projectionType;
        glm::vec2 m_center = glm::vec2(0.0f, 0.0f);
        glm::mat4 m_scaleMatrix = glm::mat4(1.0f);
        glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
        glm::mat4 m_rotationMatrix = glm::mat4(1.0f);
        GLuint m_vaoId;
    };
}

#endif // MAP_RENDERER_H