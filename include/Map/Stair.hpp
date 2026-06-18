#ifndef STAIR_HPP
#define STAIR_HPP

#include "Util/Logger.hpp"
#include <glm/vec2.hpp>


class Map;

class Stair {
public:
    static void CheckAndTransport(glm::vec2& playerPos, Map& map, const glm::vec2& offsetDir);
};

#endif