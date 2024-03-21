#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <vector>
#include <proj.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class DataLoader : public QObject
{
    Q_OBJECT

public:
    DataLoader(QObject *parent = nullptr);

    std::vector<glm::vec3> getVertices() const { return m_vertices; }

private:
    std::vector<glm::vec3> m_vertices;
};

#endif // DATA_LOADER_H