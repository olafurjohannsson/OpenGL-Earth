

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
        m_centerX = m_viewportSize.width() / 2.0f;
        m_centerY = m_viewportSize.height() / 2.0f;
        m_window->update();
    }
    void Renderer::zoomOut(int x, int y)
    {
        m_zoomFactor /= 1.1f;
        m_centerX = m_viewportSize.width() / 2.0f;
        m_centerY = m_viewportSize.height() / 2.0f;
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

    void Renderer::pan(const glm::vec2 &offset)
    {
        m_centerX += offset.x;
        m_centerY += offset.y;
        // Convert the offset from Cartesian coordinates to spherical coordinates
        double deltaLongitude = offset.x / (m_zoomFactor * 6371.0);
        double deltaLatitude = offset.y / (m_zoomFactor * 6371.0);

        // Apply the offset to the center of the view
        m_centerLongitude += glm::degrees(deltaLongitude);
        m_centerLatitude += glm::degrees(deltaLatitude);

        // Ensure the longitude is within [-180, 180]
        if (m_centerLongitude > 180.0)
            m_centerLongitude -= 360.0;
        else if (m_centerLongitude < -180.0)
            m_centerLongitude += 360.0;

        // Ensure the latitude is within [-90, 90]
        if (m_centerLatitude > 90.0)
            m_centerLatitude = 90.0;
        else if (m_centerLatitude < -90.0)
            m_centerLatitude = -90.0;

        m_window->update();
    }


    void Renderer::updateMatrix()
    {
        float width = m_viewportSize.width();
        float height = m_viewportSize.height();

        float left = m_centerX - m_zoomFactor * width / 2.0f;
        float right = m_centerX + m_zoomFactor * width / 2.0f;
        float bottom = m_centerY - m_zoomFactor * height / 2.0f;
        float top = m_centerY + m_zoomFactor * height / 2.0f;
        glm::mat4 projectionMatrix = glm::ortho(left, right, bottom, top);

        m_matrix = projectionMatrix;

        // float fov = glm::radians(45.0f); // 45 degrees
        // float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        // float nearPlane = 0.1f;
        // float farPlane = 100.0f;

        // glm::mat4 perspectiveMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

        // // Create a scale matrix for zooming
        // glm::mat4 projection = glm::ortho(0.f, width, 0.f, height);
        // glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoomFactor, m_zoomFactor, 1.0f));
        // glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-m_centerX * m_zoomFactor, -m_centerY * m_zoomFactor, 0.f));
        // glm::mat4 matrix = projection * scaleMatrix * translationMatrix;
    }

    void Renderer::paint()
    {
        m_window->beginExternalCommands();
        m_program->bind();
        m_program->enableAttributeArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        updateMatrix();

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