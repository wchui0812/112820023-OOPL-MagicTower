#include "Map/Stair.hpp"
#include "Map/Map.hpp"

namespace {
constexpr int kLayer20 = 20;
constexpr int kLayer21 = 21;
constexpr int kLayer22 = 22;
constexpr int kLayer23 = 23;
constexpr int kLayer23_1 = 25;
constexpr int kLayer23_3 = 26;
constexpr int kLayer24 = 24;
constexpr float kMoveDist = 56.0f;

bool IsInsideMap(const glm::vec2& pos) {
    constexpr float startX = -165.0f;
    constexpr float startY = 308.0f;
    constexpr float mapSize = 11.0f * kMoveDist;

    return pos.x >= startX && pos.x <= (startX + mapSize) &&
           pos.y <= startY && pos.y >= (startY - mapSize);
}
}

void Stair::CheckAndTransport(glm::vec2& playerPos, Map& map, const glm::vec2& moveVec) {
    const int tileType = map.GetTileType(playerPos.x, playerPos.y);
    if (tileType != 5 && tileType != 4) return;

    auto moveNearStair = [&](const glm::vec2& targetStair) {
        const glm::vec2 preferredPos = targetStair + (moveVec * kMoveDist);
        if (IsInsideMap(preferredPos) && map.IsWalkable(preferredPos.x, preferredPos.y)) {
            playerPos = preferredPos;
            return;
        }

        const glm::vec2 dirs[] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
        for (const auto& dir : dirs) {
            const glm::vec2 altPos = targetStair + (dir * kMoveDist);
            if (IsInsideMap(altPos) && map.IsWalkable(altPos.x, altPos.y)) {
                playerPos = altPos;
                return;
            }
        }

        playerPos = targetStair;
    };

    auto moveToTileOrNearby = [&](int row, int col) {
        const glm::vec2 targetPos = map.GetTileCenter(row, col);
        if (IsInsideMap(targetPos) && map.IsWalkable(targetPos.x, targetPos.y)) {
            playerPos = targetPos;
            return;
        }

        moveNearStair(targetPos);
    };

    const int currentLevel = map.GetCurrentLevel();
    const glm::ivec2 currentGrid = map.GetGridPosition(playerPos.x, playerPos.y);

    if (currentLevel == kLayer20 && tileType == 5) {
        map.SetLevel(kLayer21);
        LOG_INFO("Upstair, now is at layer {}", map.GetCurrentLevel());
        moveToTileOrNearby(5, 5);
        return;
    }

    if (currentLevel == kLayer22 && tileType == 5) {
        if (currentGrid.x == 5 && currentGrid.y == 0) {
            map.SetLevel(kLayer23_1);
            moveNearStair(map.FindTilePosition(4));
            return;
        }
        if (currentGrid.x == 5 && currentGrid.y == 10) {
            map.SetLevel(kLayer23_3);
            moveNearStair(map.FindTilePosition(4));
            return;
        }
        if (currentGrid.x == 10 && currentGrid.y == 5) {
            map.SetLevel(kLayer23);
            moveNearStair(map.FindTilePosition(4));
            return;
        }
    }

    if (currentLevel == kLayer23_1 && tileType == 4) {
        map.SetLevel(kLayer22);
        moveNearStair(map.GetTileCenter(5, 0));
        return;
    }

    if (currentLevel == kLayer23_3 && tileType == 4) {
        map.SetLevel(kLayer22);
        moveNearStair(map.GetTileCenter(5, 10));
        return;
    }

    if (currentLevel == kLayer23 && tileType == 4) {
        map.SetLevel(kLayer22);
        moveNearStair(map.GetTileCenter(10, 5));
        return;
    }

    if ((currentLevel == kLayer23_1 || currentLevel == kLayer23 || currentLevel == kLayer23_3) &&
        tileType == 5) {
        map.SetLevel(kLayer24);
        LOG_INFO("Upstair, now is at layer {}", map.GetCurrentLevel());
        moveToTileOrNearby(10, 5);
        return;
    }

    const int targetType = (tileType == 5) ? 4 : 5;
    if (tileType == 5) {
        map.NextLevel();
        LOG_INFO("Upstair, now is at layer {}", map.GetCurrentLevel());
    } else {
        map.PrevLevel();
        LOG_INFO("Downstair, now is at layer {}", map.GetCurrentLevel());
    }

    moveNearStair(map.FindTilePosition(targetType));
}
