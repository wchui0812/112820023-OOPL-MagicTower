#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include "App/BackgroundImage.hpp" // 使用你現有的背景圖片類別
#include "Map/Stair.hpp"
#include "Map/Door.hpp"
#include "Item/Item.hpp"

class Map {

public:
    Map();
    void Draw(); // 這是核心的雙層 for 迴圈所在

    void UpdateAnimation(float deltaTime);

    // 從檔案載入特定樓層
    void LoadLevel(const std::string& filePath);

    void LoadItems(const std::string& filePath);

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

    void RemoveTile(float x, float y);

    void TriggerDoorAnimation(float x, float y, int type) {
        m_DoorAnimating = true;
        m_AnimatingDoorPos = {x, y};
        m_AnimatingDoorType = type;

        // 根據類型讓對應的門物件開始播放
        if (type == 21) { m_YellowDoor.Reset(); m_YellowDoor.StartAnimation(); }
        else if (type == 22) m_BlueDoor.StartAnimation();
        else if (type == 23) m_RedDoor.StartAnimation();
        else if (type == 24) m_GreenDoor.StartAnimation();
        else if (type == 25) m_IronFence.StartAnimation();
    }


private:
    int m_CurrentLevel = 0;
    float m_TileSize = 60.0f;

    // layer：[樓層][列][行]
    std::vector<std::vector<std::vector<int>>> m_MapData;
    // 儲存物品資料：[樓層][列][行]
    std::vector<std::vector<std::vector<int>>> m_ItemData;

    BackgroundImage m_Wall;  // 牆壁物件
    BackgroundImage m_Floor; // 地板物件
    BackgroundImage m_Lava1;
    BackgroundImage m_Lava2;
    BackgroundImage m_Shine1;
    BackgroundImage m_Shine2;
    BackgroundImage m_UpStairs;   // 5: 上樓
    BackgroundImage m_DownStairs;
    Door m_YellowDoor; // 21
    Door m_BlueDoor;   // 22
    Door m_RedDoor;    // 23
    Door m_GreenDoor; // 24
    Door m_IronFence; // 25
    Item m_YellowKeyObj; // 10
    Item m_BlueKeyObj; // 11
    Item m_RedKeyObj; // 12
    Item m_CoinObj;
    Item m_RedPotionObj;  // 31
    Item m_BluePotionObj; // 32
    Item m_RubyObj;     // 33
    Item m_SapphireObj;    // 34
    Item m_SwordEObj;
    Item m_MagicKeyObj;
    Item m_ShieldCObj;
    Item m_SmallWingObj;
    Item m_CrossObj;
    Item m_WindCompassObj;
    Item m_SwordCObj;
    Item m_ShieldDObj;
    Item m_GemDiggerObj;
    Item m_BigWingObj;
    Item m_HolyWaterObj;
    Item m_SwordBObj;
    Item m_ShieldAObj;
    Item m_RebVeriObj;
    Item m_BlueVeriObj;

    float m_AnimationTimer = 0.0f;
    bool m_ShowAltFrame = false;

    bool m_DoorAnimating = false;
    glm::vec2 m_AnimatingDoorPos = {0, 0};
    int m_AnimatingDoorType = 0;
};

#endif
