

#include "Renderer.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
            m_modelMatrix = glm::mat4(1.f); // glm::scale(glm::mat4(1.0f), glm::vec3(m_zoomFactor, m_zoomFactor, 1.0f));
            // modelMatrix = glm::translate(modelMatrix, glm::vec3(-zoom_factor / 2.0f, -zoom_factor / 2.0f, 0.0f));

            m_centerX = 1280 / 2.0f; // Replace with the x-coordinate of the zoom point
            m_centerY = 960 / 2.0f;  // Replace with the y-coordinate of the zoom point

            QSGRendererInterface *rif = m_window->rendererInterface();
            Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL);

            std::cout << "Initializing OpenGL\n";
            initializeOpenGLFunctions();

            std::cout << "Creating Shader Program\n";
            m_program = new QOpenGLShaderProgram();

            const std::string vertexShaderSource = R"(
                #version 430 core
                layout (location = 0) in vec3 aPos; // Pass longitude and latitude as a vec2

                uniform mat4 mvpMatrix;
                uniform mat4 projectionMatrix;

               void main()
                {
                    // Pass the vertex coordinates directly to

                    gl_Position = mvpMatrix * vec4(aPos, 1.0);
                }
            )";
            // Fragment shader
            const std::string fragmentShaderSource = R"(
                #version 430 core
                out vec4 FragColor;
                void main()
                {
                    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
                }
            )";

            m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
            m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());

            m_program->bindAttributeLocation("aPos", 0);

            m_program->link();
        }
    }

    void Renderer::zoomIn(int x, int y)
    {
        std::cout << "Zoom Out x: " << x << " y: " << y << "\n";
        m_zoomFactor += 0.05f;
        m_window->update();
    }
    void Renderer::zoomOut(int x, int y)
    {
        std::cout << "Zoom Out x: " << x << " y: " << y << "\n";
        m_zoomFactor -= 0.05f;
        m_window->update();
    }
    Renderer::~Renderer()
    {
        std::cout << "Renderer dtor\n";
        delete m_program;
    }
    void Renderer::setVertices(const std::vector<std::vector<glm::vec3>> &vertices)
    {
        m_vertices = vertices;
    }
    void Renderer::pan()
    {
        std::cout << "pan\n";

        m_centerY += 1;

        m_window->update();
    }
    void Renderer::panLeft()
    {
        m_centerX -= 1;
        m_window->update();
    }
    void Renderer::panRight()
    {
        m_centerX += 1;
        m_window->update();
    }
    void Renderer::panUp()
    {
        m_centerY -= 1;
        m_window->update();
    }
    void Renderer::panDown()
    {
        m_centerY += 1;
        m_window->update();
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

        // View matrix
        glm::mat4 viewMatrix = glm::mat4(1.0f); // Identity matrix

        // Projection matrix
        float width = m_viewportSize.width();
        float height = m_viewportSize.height();
        std::cout << "width: " << width << " height: " << height << "\n";
        // m_modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoomFactor, m_zoomFactor, 1.0f));

        // glm::mat4 projectionMatrix = glm::ortho(m_centerX - width / 2.0f, m_centerX + width / 2.0f,
        //                                         m_centerY - height / 2.0f, m_centerY + height / 2.0f,
        //                                         -1000.0f, 1000.0f);
        // glm::mat4 projectionMatrix = glm::ortho(0.0f, width, 0.0f, height, -1000.0f, 1000.0f);

        float left = m_centerX - m_zoomFactor * width / 2.0f;
        float right = m_centerX + m_zoomFactor * width / 2.0f;
        float bottom = m_centerY - m_zoomFactor * height / 2.0f;
        float top = m_centerY + m_zoomFactor * height / 2.0f;

        glm::mat4 projectionMatrix = glm::ortho(left, right, bottom, top);

        // m_modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-m_centerX, -m_centerY, 0.f));
        // m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_zoomFactor, m_zoomFactor, 1.0f));
        // m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(m_centerX, m_centerY, 0.f));

        // Combine into a single matrix
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * m_modelMatrix;

        // glViewport(0, 480 - 200, 200, 200);
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        GLint location = glGetUniformLocation(m_program->programId(), "projectionMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        GLint mvpLocation = glGetUniformLocation(m_program->programId(), "mvpMatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

        for (std::vector<glm::vec3> polygon : m_vertices)
        {
            m_program->setAttributeArray(0, GL_FLOAT, polygon.data(), 3);

            // Draw the triangle
            glDrawArrays(GL_LINE_STRIP, 0, polygon.size());
        }

        m_program->disableAttributeArray(0);
        m_program->release();

        m_window->endExternalCommands();
    }
}