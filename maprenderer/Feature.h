
#ifndef FEATURE_H
#define FEATURE_H

#include <string>
#include <vector>
#include <memory>

class Geometry
{
public:
    Geometry(const std::string &type) : m_type(type)
    {
    }

private:
    std::string m_type;
};

class Feature
{
public:
    /**
     * Create a new feature, a name is optional
     */
    Feature(const std::string &geometryType)
        : m_geometry(std::make_unique<Geometry>(geometryType))
    {
    }

    void setName(const std::string &name)
    {
        m_name = name;
    }
    std::string getName() const
    {
        return m_name;
    }

private:
    std::string m_name;
    std::unique_ptr<Geometry> m_geometry{nullptr};
};

#endif