#include "DataLoader.h"

DataLoader::DataLoader(QObject *parent) : QObject(parent)
{
    PJ *P = proj_create_crs_to_crs(PJ_DEFAULT_CTX, "EPSG:4326", "EPSG:32632", NULL);
    if (P == NULL)
    {
        std::cout << "Failed to create transformation: " << proj_errno_string(proj_errno(P)) << "\n";
        return;
    }
    QFile jsonFile("../data/large.json");
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open save file.");
    }
    assert(jsonFile.exists());
    QByteArray data = jsonFile.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray features = doc.object().value("features").toArray();
    std::cout << "total feature: " << features.size() << "\n";
    for (const QJsonValue &feature : features)
    {
        QJsonObject properties = feature.toObject().value("properties").toObject();
        QString name = properties.value("name").toString();
        if (name == "Iceland")
        {
            std::cout << "Iceland found\n";
            QJsonObject geometry = feature.toObject().value("geometry").toObject();
            QString type = geometry.value("type").toString();
            QJsonArray coordinates = geometry.value("coordinates").toArray();

            std::cout << "name: " << name.toStdString() << "\n";
            std::cout << "type: " << type.toStdString() << "\n";
            if (true || type == "Polygon")
            {
                // Assuming easting_min, easting_max, northing_min, northing_max are the minimum and maximum values of your UTM coordinates
                double easting_min = std::numeric_limits<double>::max();
                double easting_max = std::numeric_limits<double>::lowest();
                double northing_min = std::numeric_limits<double>::max();
                double northing_max = std::numeric_limits<double>::lowest();

                // Create a scaling matrix to normalize the UTM coordinates to the range [0, 1]
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / (easting_max - easting_min), 1.0f / (northing_max - northing_min), 1.0f));

                // Create a translation matrix to shift the normalized coordinates to the range [-1, 1]
                glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f));

                // std::cout << "coordinates: " << coordinates.size() << "\n";

                // for (const QJsonValue &coordinate : coordinates)
                // {
                //     std::cout << "coordinate: " << coordinate.toArray().size() << "\n";
                //     for (const QJsonValue &innerCoordinates : coordinate.toArray())
                //     {
                //         std::cout << "point: " << innerCoordinates.toArray().size() << "\n";
                //         for (const QJsonValue &point : innerCoordinates.toArray())
                //         {
                //             const QJsonArray pointArray = point.toArray();
                //             std::cout << "point: " << pointArray.at(0).toDouble() << " " << pointArray.at(1).toDouble() << "\n";
                //         }
                //     }
                // }

                for (const QJsonValue &coordinate : coordinates)
                {
                    for (const QJsonValue &innerCoordinates : coordinate.toArray())
                    {
                        for (const QJsonValue &point : innerCoordinates.toArray())
                        {
                            const QJsonArray pointArray = point.toArray();
                            double lon = pointArray.at(0).toDouble();
                            double lat = pointArray.at(1).toDouble();

                            PJ_COORD a, b;
                            a = proj_coord(lon, lat, 0, 0);
                            b = proj_trans(P, PJ_FWD, a);
                            if (b.xyzt.x == HUGE_VAL)
                            {
                                std::cout << "Transformation failed\n";
                                return;
                            }

                            easting_min = std::min(easting_min, b.enu.e);
                            easting_max = std::max(easting_max, b.enu.e);
                            northing_min = std::min(northing_min, b.enu.n);
                            northing_max = std::max(northing_max, b.enu.n);
                        }
                    }
                }
                std::cout << "easting_min: " << easting_min << "\n";
                std::cout << "easting_max: " << easting_max << "\n";
                std::cout << "northing_min: " << northing_min << "\n";
                std::cout << "northing_max: " << northing_max << "\n";
                
                for (const QJsonValue &coordinate : coordinates)
                {
                    for (const QJsonValue &innerCoordinates : coordinate.toArray())
                    {
                        for (const QJsonValue &point : innerCoordinates.toArray())
                        {
                            const QJsonArray pointArray = point.toArray();
                            // QJsonArray point = coordinate.toArray();
                            double lon = pointArray.at(0).toDouble();
                            double lat = pointArray.at(1).toDouble();

                            PJ_COORD a, b;
                            a = proj_coord(lon, lat, 0, 0);
                            b = proj_trans(P, PJ_FWD, a);
                            if (b.xyzt.x == HUGE_VAL)
                            {
                                std::cout << "Transformation failed\n";
                                return;
                            }
                            // std::cout << "point: " << point.at(0).toDouble() << " " << point.at(1).toDouble() << "\n";
                            // std::cout << "point: " << b.enu.e << " " << b.enu.n << "\n";

                            // Create a glm::vec3 from your UTM coordinates
                            glm::vec3 utm_coords(b.enu.e, b.enu.n, 0.0f);

                            // Normalize the UTM coordinates to the range [0, 1]
                            glm::vec3 normalized_coords = (utm_coords - glm::vec3(easting_min, northing_min, 0.0f)) / glm::vec3(easting_max - easting_min, northing_max - northing_min, 1.0f);

                            // Translate the normalized coordinates to the range [-1, 1]
                            glm::vec3 opengl_coords = normalized_coords * 2.0f - glm::vec3(1.0f, 1.0f, 0.0f);

                            // Now opengl_coords contains your OpenGL coordinates
                            std::cout << "OpenGL coordinates: " << opengl_coords.x << " " << opengl_coords.y << "\n";

                            m_vertices.push_back(opengl_coords);
                        }
                    }
                }
            }
        }
    }
    proj_destroy(P);
}