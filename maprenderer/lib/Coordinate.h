#ifndef COORDINATE_H
#define COORDINATE_H

#include <QJsonValue>
#include <QJsonArray>

class Coordinate
{
public:
    Coordinate() = default;

    Coordinate(double longitude, double latitude);

    static Coordinate fromJson(const QJsonValue &json);

    double getLongitude() const;
    double getLatitude() const;
    

private:
    double m_longitude;
    double m_latitude;
    
};

#endif