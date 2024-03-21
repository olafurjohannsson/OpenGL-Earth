

#include "Renderer.h"
#include <iostream>

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

    void Renderer::init()
    {
        if (nullptr == m_program)
        {
            std::cout << "init\n";
            QSGRendererInterface *rif = m_window->rendererInterface();
            Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL);

            std::cout << "Initializing OpenGL\n";
            initializeOpenGLFunctions();

            std::cout << "Creating Shader Program\n";
            m_program = new QOpenGLShaderProgram();
            m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                        "attribute highp vec4 vertices; void main() { gl_Position = vertices; }");
            m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                        "void main() { gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }");

            m_program->bindAttributeLocation("vertices", 0);

            m_program->link();
        }
    }

    Renderer::~Renderer()
    {
        std::cout << "Renderer dtor\n";
        delete m_program;
    }
    void Renderer::setVertices(const std::vector<glm::vec3> &vertices)
    {
        m_vertices = vertices;
    }
    void Renderer::paint()
    {
        std::cout << "paint " << m_viewportSize.width() << " " << m_viewportSize.height() << "\n";
        // Play nice with the RHI. Not strictly needed when the scenegraph uses
        // OpenGL directly.
        m_window->beginExternalCommands();

        m_program->bind();

        m_program->enableAttributeArray(0);

        // Define the vertices of the triangle
        

        // This example relies on (deprecated) client-side pointers for the vertex
        // input. Therefore, we have to make sure no vertex buffer is bound.
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Set the attribute array for the vertex positions
        m_program->setAttributeArray(0, GL_FLOAT, m_vertices.data(), 3);
        

        // glViewport(0, 480 - 200, 200, 200);
         glViewport(0, 0, 640, 480);
        glClearColor(0, 0, 0, 1);

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        // Draw the triangle
        glDrawArrays(GL_LINE_STRIP, 0, m_vertices.size());

        m_program->disableAttributeArray(0);
        m_program->release();

        m_window->endExternalCommands();
    }
}