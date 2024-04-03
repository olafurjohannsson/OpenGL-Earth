

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

        std::vector<glm::vec2> allVertices;
        std::vector<unsigned int> allIndices;
        unsigned int currentIndex = 0;
        unsigned int indexOffset = 0;
        unsigned int vertexOffset = 0;
        for (Polygon &polygon : m_polygons)
        {
            const std::vector<glm::vec2> &vertices = polygon.vertices();
            allVertices.insert(allVertices.end(), vertices.begin(), vertices.end());

            if (polygon.isTriangulated())
            {
                const std::vector<unsigned int> &indices = polygon.indices();
                
                for (unsigned int index : indices)
                {
                    allIndices.push_back(index + currentIndex);
                }
            }
            
            polygon.setOffset(vertexOffset, indexOffset);
            currentIndex += vertices.size();
            vertexOffset += polygon.size();
            indexOffset += polygon.indices().size();
        }
        // Create and bind the VAO
        GLuint vao;
        OpenGLInterface::get()->glGenVertexArrays(1, &vao);
        OpenGLInterface::get()->glBindVertexArray(vao);

        // Create the VBO, upload the vertex data, and set the vertex attribute pointer
        GLuint vbo;
        OpenGLInterface::get()->glGenBuffers(1, &vbo);
        OpenGLInterface::get()->glBindBuffer(GL_ARRAY_BUFFER, vbo);
        OpenGLInterface::get()->glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(glm::vec2), allVertices.data(), GL_STATIC_DRAW);
        OpenGLInterface::get()->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        OpenGLInterface::get()->glEnableVertexAttribArray(0);

        // Create the EBO and upload the index data
        GLuint ebo;
        OpenGLInterface::get()->glGenBuffers(1, &ebo);
        OpenGLInterface::get()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        OpenGLInterface::get()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(unsigned int), allIndices.data(), GL_STATIC_DRAW);

        // Unbind the VAO
        OpenGLInterface::get()->glBindVertexArray(0);

        // Store the VAO ID
        m_vaoId = vao;
    }

    void Renderer::updateUniforms(const glm::vec2 &center,
                                  const glm::mat4 &projectionMatrix,
                                  const glm::mat4 &scaleMatrix,
                                  const glm::mat4 &rotationMatrix)
    {
        m_projectionMatrix = projectionMatrix;
        m_scaleMatrix = scaleMatrix;
        m_rotationMatrix = rotationMatrix;
        m_center = center;

        if (m_window)
        {
            m_window->update();
        }
    }

    void Renderer::render()
    {
        m_window->beginExternalCommands();

        OpenGLInterface::get()->glUseProgram(m_program->programId());

        OpenGLInterface::get()->glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
        OpenGLInterface::get()->glDisable(GL_DEPTH_TEST);

        OpenGLInterface::get()->glEnable(GL_BLEND);
        OpenGLInterface::get()->glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        OpenGLInterface::get()->glClearColor(1 / 255.f, 19 / 255.f, 34 / 255.f, 1.f);
        OpenGLInterface::get()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLint projectionMatrixLocation = OpenGLInterface::get()->glGetUniformLocation(m_program->programId(), "projectionMatrix");
        OpenGLInterface::get()->glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

        GLint scaleMatrixLocation = OpenGLInterface::get()->glGetUniformLocation(m_program->programId(), "scaleMatrix");
        OpenGLInterface::get()->glUniformMatrix4fv(scaleMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_scaleMatrix));

        GLint rotationMatrixLocation = OpenGLInterface::get()->glGetUniformLocation(m_program->programId(), "rotationMatrix");
        OpenGLInterface::get()->glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_rotationMatrix));

        GLint centerLocation = OpenGLInterface::get()->glGetUniformLocation(m_program->programId(), "center");
        OpenGLInterface::get()->glUniform2f(centerLocation, glm::radians(m_center.x), glm::radians(m_center.y));

        GLint projectionTypeLocation = OpenGLInterface::get()->glGetUniformLocation(m_program->programId(), "projectionType");
        OpenGLInterface::get()->glUniform1i(projectionTypeLocation, static_cast<int>(m_projectionType));

        // Bind the VAO
        OpenGLInterface::get()->glBindVertexArray(m_vaoId);

        // Draw the polygons
        for (const Polygon &polygon : m_polygons)
        {
            // Draw the outline
            OpenGLInterface::get()->glDrawArrays(GL_LINE_STRIP, polygon.vertexOffset(), polygon.size());
            if (polygon.isTriangulated())
            {
                // Draw the triangles
                OpenGLInterface::get()->glDrawElements(GL_TRIANGLES,
                                                       polygon.indices().size(),
                                                       GL_UNSIGNED_INT,
                                                       (void *)(polygon.indexOffset() * sizeof(unsigned int)));
            }
        }

        // Unbind the VAO
        OpenGLInterface::get()->glBindVertexArray(0);

        m_program->release();

        m_window->endExternalCommands();
    }
}