#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include "App/BackgroundImage.hpp"
#include "Map/Stair.hpp"
#include "Map/Door.hpp"
#include "Item/Item.hpp"
#include "Enemy/Enemy.hpp"
#include "NPC/NPC.hpp"

#include "Util/Renderer.hpp"

class Map {

public:
    Map();
    void Reset();
    void Draw();

    void UpdateAnimation(float deltaTime);

    // 從檔案載入特定樓層
    void LoadLevel(const std::string& filePath);

    void LoadItems(const std::string& filePath);


    // 1. 取得座標點的格子類型
    int GetTileType(float x, float y) const;

    // 2. 樓層切換
    void NextLevel() {
        if (m_CurrentLevel < 24 && m_CurrentLevel + 1 < (int)m_MapData.size()) {
            m_CurrentLevel++;
            this->InitLevelEnemies(); // 確保這裡有執行
            this->InitLevelNPCs();
            LOG_INFO("Switched to Level {}", m_CurrentLevel);
        }
    }
    void PrevLevel() {
        if (m_CurrentLevel > 0) {
            m_CurrentLevel--;
            this->InitLevelEnemies(); // 確保這裡有執行
            this->InitLevelNPCs();
        }
    }

    // 3. 取得當前樓層 (用於 Log 顯示)
    int GetCurrentLevel() const {
        return m_CurrentLevel;
    }

    int GetDisplayLevel() const;
    std::string GetDisplayLevelName() const;

    int GetLevelCount() const {
        return static_cast<int>(m_MapData.size());
    }

    void SetFinalSealReleased(bool released) {
        m_FinalSealReleased = released;
        InitLevelEnemies();
    }

    bool IsFinalSealReleased() const {
        return m_FinalSealReleased;
    }

    // 控制樓層
    void SetLevel(const int level) {
        m_CurrentLevel = level;
        InitLevelEnemies();
        InitLevelNPCs();
    }

    // 檢查目標位置是否可以通行
    bool IsWalkable(float x, float y) const;

    glm::ivec2 GetGridPosition(float x, float y) const;
    glm::vec2 GetTileCenter(int row, int col) const;

    glm::vec2 FindTilePosition(int targetType) const;

    void RemoveTile(float x, float y);
    void SetTileAtLevel(int level, int row, int col, int tileType);
    void RemoveTileAtLevel(int level, int row, int col);

    void TriggerDoorAnimation(float x, float y, int type, int replacementTile = 0) {
        m_DoorAnimating = true;
        m_AnimatingDoorPos = {x, y};
        m_AnimatingDoorType = type;
        m_DoorReplacementTile = replacementTile;

        // 根據類型讓對應的門物件開始播放
        if (type == 21) { m_YellowDoor.Reset(); m_YellowDoor.StartAnimation(); }
        else if (type == 22) m_BlueDoor.StartAnimation();
        else if (type == 23) m_RedDoor.StartAnimation();
        else if (type == 24) m_GreenDoor.StartAnimation();
        else if (type == 25) m_IronFence.StartAnimation();
    }

    // 1. 宣告 LoadEnemies 函式供外部呼叫
    void LoadEnemies(const std::string& filePath);

    // 2. 宣告初始化當前樓層敵人的函式
    void InitLevelEnemies();

    std::shared_ptr<Enemy> GetEnemyAt(float x, float y);

    // 將敵人從當前樓層移除
    void RemoveEnemy(std::shared_ptr<Enemy> enemy);

    // 3. 儲存所有樓層的原始數據：[樓層][列][行]
    std::vector<std::vector<std::vector<int>>> m_EnemyRawData;

    std::vector<std::vector<std::vector<int>>> m_NPCRawData;

    // 4. 儲存當前樓層生成的敵人物件實體
    const std::vector<std::shared_ptr<Enemy>>& GetEnemies() const { return m_Enemies; }

    void LoadNPCs(const std::string& filePath);
    void InitLevelNPCs();
    const std::vector<std::shared_ptr<NPC>>& GetNPCs() const { return m_NPCs; }
    std::shared_ptr<NPC> GetNPCAt(float x, float y);

    void MoveNPC(std::shared_ptr<NPC> npc, int colOffset, int rowOffset);
    void RemoveNPC(std::shared_ptr<NPC> npc);


private:
    struct NPCVisualPart {
        std::shared_ptr<BackgroundImage> object;
        std::string imagePath1;
        std::string imagePath2;
    };

    int m_CurrentLevel = 0;
    float m_StartX = -165.0f;
    float m_StartY = 308.0f;
    float m_TileSize = 56.0f;

    // layer：[樓層][列][行]
    std::vector<std::vector<std::vector<int>>> m_MapData;
    // 儲存物品資料：[樓層][列][行]
    std::vector<std::vector<std::vector<int>>> m_ItemData;

    std::vector<std::shared_ptr<Enemy>> m_Enemies;

    std::vector<std::shared_ptr<NPC>> m_NPCs;
    std::vector<NPCVisualPart> m_NPCVisualParts;


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
    Item m_GreenVeriObj;
    Item m_GodKnifeObj;

    float m_AnimationTimer = 0.0f;
    bool m_ShowAltFrame = false;

    bool m_DoorAnimating = false;
    glm::vec2 m_AnimatingDoorPos = {0, 0};
    int m_AnimatingDoorType = 0;
    int m_DoorReplacementTile = 0;
    bool m_FinalSealReleased = false;


};

#endif
