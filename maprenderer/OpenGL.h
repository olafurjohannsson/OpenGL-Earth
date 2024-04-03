#ifndef OPENGL_H
#define OPENGL_H

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLVersionFunctionsFactory>

class OpenGLInterface : public QOpenGLFunctions_4_3_Core
{
public:
    static OpenGLInterface *get()
    {
        static OpenGLInterface *instance = nullptr;
        if (!instance)
        {
            instance = new OpenGLInterface();
        }
        return instance;
    }

private:
    OpenGLInterface()
    {
        initializeOpenGLFunctions();
    }
};
#endif // OPENGL_H