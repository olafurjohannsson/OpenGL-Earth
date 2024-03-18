

#include "Renderer.h"

namespace Map
{
    Renderer::Renderer(QQuickItem *parent)
        : QObject(parent)
    {
        // setFlag(ItemHasContents, true);
    }

    void Renderer::paint()
    {
        if (!m_program)
        {
            m_program = new QOpenGLShaderProgram();
            m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
            m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl");
            m_program->link();
        }

        m_program->bind();

        m_program->setUniformValue("viewportSize", m_viewportSize);
        m_program->setUniformValue("clearColor", m_clearColor);
        m_program->setUniformValue("rotation", m_rotation);
        m_program->setUniformValue("zoom", m_zoom);
        m_program->setUniformValue("center", m_center);

        glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
        glClearColor(m_clearColor.redF(), m_clearColor.greenF(), m_clearColor.blueF(), m_clearColor.alphaF());
        glClear(GL_COLOR_BUFFER_BIT);

        m_program->release();
    }

    void Renderer::init()
    {
        if (!m_program)
        {
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
    }

    void Renderer::setViewportSize(const QSize &size)
    {
        m_viewportSize = size;
    }

    void Renderer::setClearColor(const QColor &color)
    {
        m_clearColor = color;
    }

    void Renderer::setRotation(float angle)
    {
        m_rotation = angle;
    }

    void Renderer::setZoom(float zoom)
    {
        m_zoom = zoom;
    }

    void Renderer::setCenter(const QPointF &center)
    {
        m_center = center;
    }

    void Renderer::render()
    {
        if (!m_program)
        {
            m_program = new QOpenGLShaderProgram();
            m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
            m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl");
            m_program->link();
        }

        m_program->bind();

        m_program->setUniformValue("viewportSize", m_viewportSize);
        m_program->setUniformValue("clearColor", m_clearColor);
        m_program->setUniformValue("rotation", m_rotation);
        m_program->setUniformValue("zoom", m_zoom);
        m_program->setUniformValue("center", m_center);

        glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
        glClearColor(m_clearColor.redF(), m_clearColor.greenF(), m_clearColor.blueF(), m_clearColor.alphaF());
        glClear(GL_COLOR_BUFFER_BIT);

        m_program->release();
    }
}