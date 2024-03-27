
#include "Coordinate.h"

Coordinate::Coordinate(double longitude, double latitude)
    :  m_longitude(longitude), m_latitude(latitude)
{
    assert(latitude >= -90 && latitude <= 90);
    assert(longitude >= -180 && longitude <= 180);
}

Coordinate Coordinate::fromJson(const QJsonValue &json)
{
    double longitude = json.toArray().at(0).toDouble();
    double latitude = json.toArray().at(1).toDouble();
    return Coordinate(longitude, latitude);
}

double Coordinate::getLatitude() const
{
    return m_latitude;
}
double Coordinate::getLongitude() const
{
    return m_longitude;
}