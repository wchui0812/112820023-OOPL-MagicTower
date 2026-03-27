#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include "App/BackgroundImage.hpp" // 使用你現有的背景圖片類別
#include "Map/Stair.hpp"

class Map {
public:
    Map();
    void Draw(); // 這是核心的雙層 for 迴圈所在

    void UpdateAnimation(float deltaTime);

    // 從檔案載入特定樓層
    void LoadLevel(const std::string& filePath);

    // 1. 取得座標點的格子類型
    int GetTileType(float x, float y) const;

    // 2. 樓層切換
    void NextLevel() {
        if (m_CurrentLevel + 1 < (int)m_MapData.size()) m_CurrentLevel++;
    }
    void PrevLevel() {
        if (m_CurrentLevel > 0) m_CurrentLevel--;
    }

    // 3. 取得當前樓層 (用於 Log 顯示)
    int GetCurrentLevel() const {
        return m_CurrentLevel;
    }

    // 控制樓層
    void SetLevel(const int level) {
        m_CurrentLevel = level;
    }

    // 檢查目標位置是否可以通行
    bool IsWalkable(float x, float y) const;

    // 新增：尋找特定格子(如 4 或 5)在當前樓層的座標
    glm::vec2 FindTilePosition(int targetType) const;


private:
    int m_CurrentLevel = 0;
    float m_TileSize = 60.0f;

    // 三維向量：[樓層][列][行]
    std::vector<std::vector<std::vector<int>>> m_MapData;

    BackgroundImage m_Wall;  // 牆壁物件
    BackgroundImage m_Floor; // 地板物件
    BackgroundImage m_Lava1;
    BackgroundImage m_Lava2;
    BackgroundImage m_Shine1;
    BackgroundImage m_Shine2;
    BackgroundImage m_UpStairs;   // 5: 上樓
    BackgroundImage m_DownStairs;

    float m_AnimationTimer = 0.0f;
    bool m_ShowAltFrame = false;
};

#endif
