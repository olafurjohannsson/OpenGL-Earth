#ifndef GEOGRAPHIC_LOADER_H
#define GEOGRAPHIC_LOADER_H

#include <iostream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <vector>
#include <proj.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Projection.h"
#include "Polygon.h"

class GeographicLoader
{
public:
    GeographicLoader(const QString &filename);
    
    const std::vector<Polygon> &polygons() const;

    /**
     * Apply a projection on all polygons.
     * The data inside the Coordinate objects will be modified.
     * Projections can be re-applied because the original data is stored in the Coordinate objects.
    */
    void project(const Projection &projection);

private:
    QJsonDocument m_jsonDocument;
    std::vector<Polygon> m_polygons;
};

#endif