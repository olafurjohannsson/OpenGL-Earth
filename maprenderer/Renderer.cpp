

#include "Renderer.h"
#include "lib/Transformation.h"

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

            // Model matrix
            m_zoomFactor = 1.f;
            m_centerX = 1280 / 2.0f; // Replace with the x-coordinate of the zoom point
            m_centerY = 960 / 2.0f;  // Replace with the y-coordinate of the zoom point


            QSGRendererInterface *rif = m_window->rendererInterface();
            Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL);

            initializeOpenGLFunctions();
            m_program = new QOpenGLShaderProgram();
            assert(m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "../maprenderer/shaders/vertex.glsl"));
            assert(m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "../maprenderer/shaders/fragment.glsl"));
            m_program->bindAttributeLocation("aPos", 0);

            m_program->link();
        }
    }

    void Renderer::zoomIn(int x, int y)
    {
        m_zoomFactor *= 1.1f;
        m_window->update();
    }
    void Renderer::zoomOut(int x, int y)
    {
        m_zoomFactor /= 1.1f;
        m_window->update();
    }
    Renderer::~Renderer()
    {
        delete m_program;
    }
    void Renderer::setPolygons(const std::vector<Polygon> &polygons)
    {
        m_polygons = polygons;
    }
    void Renderer::setZoom(float zoom)
    {
        m_zoomFactor = zoom;
        if(m_window) {
            m_window->update();
        }
    }
    void Renderer::pan(const glm::vec2 &offset)
    {
        m_centerX += offset.x;
        m_centerY += offset.y;


        m_window->update();
    }


    void Renderer::updateMatrix(const glm::mat4 &matrix)
    {
        // m_matrix = matrix;
        float width = m_viewportSize.width();
        float height = m_viewportSize.height();

        float left = m_centerX - m_zoomFactor * width / 2.0f;
        float right = m_centerX + m_zoomFactor * width / 2.0f;
        float bottom = m_centerY - m_zoomFactor * height / 2.0f;
        float top = m_centerY + m_zoomFactor * height / 2.0f;
        glm::mat4 projectionMatrix = glm::ortho(left, right, bottom, top);
        // m_matrix = projectionMatrix;
        m_matrix = matrix;
        if(m_window) {
            m_window->update();
        }

        // glm::mat4 projection = glm::ortho(0.f, width, 0.f, height);
        // glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-m_centerX, -m_centerY, 0.f));
        // glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoomFactor, m_zoomFactor, 1.0f));
        // m_matrix = projection * translationMatrix * scaleMatrix;


    }
    void Renderer::test(int x, int y)
    {
        auto vec = m_matrix * glm::vec4(x, y, 0, 1);

        std::cout << "x: " << vec.x << " y: " << vec.y << "\n";
    }
    void Renderer::paint()
    {
        m_window->beginExternalCommands();
        m_program->bind();
        m_program->enableAttributeArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // updateMatrix();

        glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

        glClearColor(1 / 255.f, 19 / 255.f, 34 / 255.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glDisable(GL_DEPTH_TEST);
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        GLint location = glGetUniformLocation(m_program->programId(), "matrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_matrix));
        
        
        for (const Polygon &polygon : m_polygons)
        {
            m_program->setAttributeArray(0, GL_FLOAT, polygon.data(), 2);
            glDrawArrays(GL_LINE_STRIP, 0, polygon.size());
        }

        m_program->disableAttributeArray(0);
        m_program->release();

        m_window->endExternalCommands();
    }
}