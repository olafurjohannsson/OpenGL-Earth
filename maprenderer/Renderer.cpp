

#include "Renderer.h"

namespace Map
{
    void Renderer::setViewportSize(const QSize &size)
    {
        m_viewportSize = size;
    }
    void Renderer::setWindow(QQuickWindow *window)
    {
        m_window = window;
    }
    void Renderer::setProjectionType(Projection::ProjectionType type)
    {
        m_projectionType = type;
    }
    void Renderer::inititalize()
    {
        if (nullptr == m_program)
        {
            initializeOpenGLFunctions();
            m_program = new QOpenGLShaderProgram();
            m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "../maprenderer/shaders/vertex.glsl");
            m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "../maprenderer/shaders/fragment.glsl");
            m_program->bindAttributeLocation("vertex", 0);
            m_program->link();
        }
    }

    Renderer::~Renderer()
    {
        delete m_program;
    }

    void Renderer::setPolygons(const std::vector<Polygon> &polygons)
    {
        m_polygons = polygons;
    }

    void Renderer::updateUniforms(const glm::vec2& center, 
        const glm::mat4 &projectionMatrix, 
        const glm::mat4& scaleMatrix,
        const glm::mat4& rotationMatrix)
    {
        m_projectionMatrix = projectionMatrix;
        m_scaleMatrix = scaleMatrix;
        m_rotationMatrix = rotationMatrix;
        m_center = center;

        if(m_window) 
        {
            m_window->update();
        }
    }
    
    void Renderer::render()
    {
        m_window->beginExternalCommands();
        m_program->bind();
        m_program->enableAttributeArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glClearColor(1 / 255.f, 19 / 255.f, 34 / 255.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLint projectionMatrixLocation = glGetUniformLocation(m_program->programId(), "projectionMatrix");
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

        GLint scaleMatrixLocation = glGetUniformLocation(m_program->programId(), "scaleMatrix");
        glUniformMatrix4fv(scaleMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_scaleMatrix));
        
        GLint rotationMatrixLocation = glGetUniformLocation(m_program->programId(), "rotationMatrix");
        glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_rotationMatrix));

        GLint centerLocation = glGetUniformLocation(m_program->programId(), "center");
        glUniform2f(centerLocation, glm::radians(m_center.x), glm::radians(m_center.y));

        GLint projectionTypeLocation = glGetUniformLocation(m_program->programId(), "projectionType");
        glUniform1i(projectionTypeLocation, static_cast<int>(m_projectionType));
        
        // render lines for outline
        for (const Polygon &polygon : m_polygons)
        {
            m_program->setAttributeArray(0, GL_FLOAT, polygon.data(), 2);
            glDrawArrays(GL_LINE_STRIP, 0, polygon.size());
        }

        // render triangles for fill

        m_program->disableAttributeArray(0);
        m_program->release();

        m_window->endExternalCommands();
    }
}