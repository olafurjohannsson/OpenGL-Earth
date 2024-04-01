#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>

/**
 * This class contains the functions to transform the coordinates from the WGS84 system to screen coordinates
*/
class Transformation
{
    public:

        /**
         * Convert the lon/lat coordinates to the stereographic projection
        */
        static glm::vec2 stereographic(const glm::vec2 &radians);

        /**
         * Convert the lon/lat coordinates to the orthographic projection
        */
        static glm::vec2 orthographic(const glm::vec2 &radians);

        /**
         * Convert the lon/lat coordinates to the mercator projection
        */
        static glm::vec2 mercator(const glm::vec2 &radians);

        /**
         * Convert the screen coordinates to the lon/lat coordinates
        */
        static glm::vec2 inverseMercator(const glm::vec2& point);

        /**
         * Normalize the coordinates to the range [-1, 1]
        */
        static glm::vec2 normalize(const glm::vec2 &radians);

        /**
         * Map the normalized coordinates to the screen coordinates
        */
        static glm::vec2 map(const glm::vec2 &normalized, int width, int height);

        /**
         * Reverse the mapping from screen coordinates to normalized coordinates
        */
        static glm::vec2 unmap(const glm::vec2 &point, int width, int height);

        
};


#endif