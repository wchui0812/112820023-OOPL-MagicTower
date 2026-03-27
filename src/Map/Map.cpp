#include "Map/Map.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"

#include <fstream>
#include <iostream>

Map::Map() 
    : m_Wall(RESOURCE_DIR "/Image/Road/wall_b.bmp"),
      m_Floor(RESOURCE_DIR "/Image/Road/road.bmp"),
      m_Lava1(RESOURCE_DIR "/Image/Road/lava.bmp"),
      m_Lava2(RESOURCE_DIR "/Image/Road/lava2.bmp"),
      m_Shine1(RESOURCE_DIR "/Image/Road/wall_shine.bmp"),
      m_Shine2(RESOURCE_DIR "/Image/Road/wall_shine2.bmp"),
      m_UpStairs(RESOURCE_DIR "/Image/Background/Stair/upstair.bmp"),   // 5
      m_DownStairs(RESOURCE_DIR "/Image/Background/Stair/downstair.bmp") // 4
{
    float newScale = 0.73f;
    m_Wall.SetScale({newScale, newScale});
    m_Floor.SetScale({newScale, newScale});
    m_Lava1.SetScale({newScale, newScale});
    m_Lava2.SetScale({newScale, newScale});
    m_Shine1.SetScale({newScale, newScale});
    m_Shine2.SetScale({newScale, newScale});
    m_UpStairs.SetScale({newScale, newScale});
    m_DownStairs.SetScale({newScale, newScale});

    m_TileSize = 56.0f;

    for (int i = 0; i < 25; ++i) {
        // 動態組合路徑：D:/.../Resources/Levels/map0.txt, map1.txt ...
        std::string filePath = RESOURCE_DIR "/Layer/layer" + std::to_string(i) + ".txt";

        // 呼叫原本的 LoadLevel 函式
        LoadLevel(filePath);
    }

    m_CurrentLevel = 0;
}

void Map::UpdateAnimation(float deltaTime) {
    m_AnimationTimer += deltaTime;
    if (m_AnimationTimer >= 0.5f) { // 每 0.5 秒切換一次
        m_ShowAltFrame = !m_ShowAltFrame;
        m_AnimationTimer = 0.0f;
    }
}

void Map::LoadLevel(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        LOG_ERROR("無法開啟地圖檔: {}", filePath);
        return;
    }

    std::vector<std::vector<int>> tempLevel(11, std::vector<int>(11));
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            // 直接將檔案中的數字讀入二維陣列
            if (!(file >> tempLevel[i][j])) {
                LOG_ERROR("地圖檔案格式錯誤: {}", filePath);
                // 處理格式不符的情況
            }
        }
    }
    m_MapData.push_back(tempLevel);
    file.close();

    LOG_INFO("成功載入樓層: {}", filePath);
}

void Map::Draw() {
    if (m_MapData.empty()) return;
    Util::Renderer renderer;

    float startX = -165.0f;
    float startY = 308.0f;

    auto& currentMap = m_MapData[m_CurrentLevel];

    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            float posX = startX + (j * m_TileSize) + (m_TileSize / 2.0f);
            float posY = startY - (i * m_TileSize) - (m_TileSize / 2.0f);

            int tileType = currentMap[i][j];

            if (tileType != 0 && tileType != 1 && tileType != 2 && tileType != 3 && tileType != 4 && tileType != 5) {
                LOG_DEBUG("座標 ({}, {}) 讀到了未知數字: {}", i, j, tileType);
            }

            BackgroundImage* target = nullptr; // 用指標暫存要畫的圖片

            // --- 圖片選取邏輯 ---
            if (tileType == 1) { // 牆壁
                m_Wall.SetPosition({posX, posY});
                m_Wall.SetZIndex(1.0f);
                renderer.AddChild(std::make_shared<BackgroundImage>(m_Wall));
            }
            else if (tileType == 2) { // 岩漿動畫
                // 根據 m_ShowAltFrame 決定畫哪張
                //BackgroundImage& lava = m_ShowAltFrame ? m_Lava2 : m_Lava1;
                //lava.SetPosition({posX, posY});
                //lava.SetZIndex(1.0f);
                //renderer.AddChild(std::shared_ptr<Util::GameObject>(&lava, [](Util::GameObject*){}));
                if (m_ShowAltFrame) {
                    m_Lava2.SetPosition({posX, posY});
                    m_Lava2.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<BackgroundImage>(m_Lava2));
                } else {
                    m_Lava1.SetPosition({posX, posY});
                    m_Lava1.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<BackgroundImage>(m_Lava1));
                }
            }
            else if (tileType == 3) { // 閃爍牆動畫
                //BackgroundImage& shine = m_ShowAltFrame ? m_Shine2 : m_Shine1;
                //shine.SetPosition({posX, posY});
                //shine.SetZIndex(1.0f);
                //renderer.AddChild(std::shared_ptr<Util::GameObject>(&shine, [](Util::GameObject*){}));
                if (m_ShowAltFrame) {
                    m_Shine2.SetPosition({posX, posY});
                    m_Shine2.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<BackgroundImage>(m_Shine2));
                } else {
                    m_Shine1.SetPosition({posX, posY});
                    m_Shine1.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<BackgroundImage>(m_Shine1));
                }
            }
            else if (tileType == 4) { // 下樓梯
                m_UpStairs.SetPosition({posX, posY});
                m_UpStairs.SetZIndex(1.0f);
                renderer.AddChild(std::make_shared<BackgroundImage>(m_UpStairs));
            }
            else if (tileType == 5) { // 上樓梯
                m_DownStairs.SetPosition({posX, posY});
                m_DownStairs.SetZIndex(1.0f);
                renderer.AddChild(std::make_shared<BackgroundImage>(m_DownStairs));
            }
            else {
                m_Floor.SetPosition({posX, posY});
                m_Floor.SetZIndex(1.0f);
                renderer.AddChild(std::make_shared<BackgroundImage>(m_Floor));
            }

        }
    }
    renderer.Update();
}

int Map::GetTileType(float x, float y) const {
    float startX = -165.0f;
    float startY = 308.0f;
    float tileSize = 56.0f;

    // 換算成索引
    int col = static_cast<int>((x - startX) / tileSize);
    int row = static_cast<int>((startY - y) / tileSize);

    // 邊界安全檢查
    if (row < 0 || row >= 11 || col < 0 || col >= 11) {
        return -1; // 超出範圍
    }

    return m_MapData[m_CurrentLevel][row][col];
}

bool Map::IsWalkable(float x, float y) const {
    // 1. 計算偏移量（與 Draw 函式一致）
    float startX = -165.0f;
    float startY = 308.0f;
    float mapSize = 11.0f * m_TileSize;

    if (x < startX || x >= (startX + mapSize) ||
        y > startY || y <= (startY - mapSize)) {
        return false; // 直接擋掉，不讓它進去算索引
        }

    // 2. 將螢幕座標 (x, y) 轉換為陣列索引 (row, col)
    int col = static_cast<int>((x - startX + 0.1f) / m_TileSize);
    int row = static_cast<int>((startY - y + 0.1f) / m_TileSize);

    // 3. 邊界檢查：超出 11x11 範圍視為不能走
    if (row < 0 || row >= 11 || col < 0 || col >= 11) {
        return false;
    }

    // 取得當前樓層的格子數值
    int tileType = m_MapData[m_CurrentLevel][row][col];

    // 4. 通行判斷：地板(0)、下樓梯(4)、上樓梯(5)
    return (tileType == 0 || tileType == 4 || tileType == 5);
}

glm::vec2 Map::FindTilePosition(int targetType) const {
    auto& currentMap = m_MapData[m_CurrentLevel];
    float startX = -165.0f;
    float startY = 308.0f;
    float tileSize = 56.0f;

    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            if (m_MapData[m_CurrentLevel][i][j] == targetType) {
                float posX = startX + (j * tileSize) + (tileSize / 2.0f);
                float posY = startY - (i * tileSize) - (tileSize / 2.0f);
                return {posX, posY};
            }
        }
    }
    return {0.0f, 0.0f}; // 若沒找到則回傳中心點
}