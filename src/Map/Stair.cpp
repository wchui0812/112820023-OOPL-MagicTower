#include "Map/Stair.hpp"
#include "Map/Map.hpp"

void Stair::CheckAndTransport(glm::vec2& playerPos, Map& map, const glm::vec2& moveVec) {
    int tileType = map.GetTileType(playerPos.x, playerPos.y);
    float moveDist = 56.0f; // 你的格子大小

    LOG_DEBUG("Player is standing on Tile Type: {}", tileType);

    if (tileType == 5 || tileType == 4) {
        int targetType = (tileType == 5) ? 4 : 5; // 上樓找下樓梯(4)，下樓找上樓梯(5)

        if (tileType == 5) {
            map.NextLevel();
            LOG_INFO("Upstair, now is at layer：{}", map.GetCurrentLevel() + 1);
        } else {
            map.PrevLevel();
            LOG_INFO("Downstair, now is at layer：{}", map.GetCurrentLevel() + 1);
        }

        // 1. 先找到目標樓梯在該層的中心座標
        glm::vec2 targetStair = map.FindTilePosition(targetType);

        // 2. 【這就是你要加的那段邏輯】
        // 嘗試照著原本的方向推一格
        glm::vec2 nextPos = targetStair + (moveVec * moveDist);

        float startX = -165.0f;
        float startY = 308.0f;
        float tileSize = 56.0f;
        float mapSize = 11.0f * tileSize;

        // 定義新的地圖邊界範圍
        auto isInside = [&](const glm::vec2& pos) {
            return pos.x >= startX && pos.x <= (startX + mapSize) &&
                   pos.y <= startY && pos.y >= (startY - mapSize);
        };

        // 使用新邊界進行落腳點檢查
        if (isInside(nextPos) && map.IsWalkable(nextPos.x, nextPos.y)) {
            playerPos = nextPos;
        } else {
            // 如果原本方向會出界或撞牆，嘗試其他方向
            glm::vec2 dirs[] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
            bool found = false;
            for (auto& d : dirs) {
                glm::vec2 altPos = targetStair + (d * moveDist);
                if (isInside(altPos) && map.IsWalkable(altPos.x, altPos.y)) {
                    playerPos = altPos;
                    found = true;
                    break;
                }
            }
            if (!found) playerPos = targetStair; // 真的沒地方去才疊在樓梯上
        }
    }
}