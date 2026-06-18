#include "Map/Map.hpp"

#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

#include <fstream>
#include <algorithm>
#include <string>

namespace {
constexpr int kLayer23_1Index = 25;
constexpr int kLayer23_3Index = 26;

void DrawLargeEnemy(Util::Renderer& renderer, const Enemy& enemy, const std::string& imagePrefix, float tileSize) {
    const glm::vec2 markerPos = enemy.m_Transform.translation;

    for (int part = 1; part <= 9; ++part) {
        const int localRow = (part - 1) / 3;
        const int localCol = (part - 1) % 3;
        const int markerLocalRow = 2;
        const int markerLocalCol = 1;

        const int rowOffset = localRow - markerLocalRow;
        const int colOffset = localCol - markerLocalCol;
        const glm::vec2 partPos = markerPos + glm::vec2(colOffset * tileSize, -rowOffset * tileSize);

        auto partImage = std::make_shared<BackgroundImage>(
            std::string(RESOURCE_DIR "/Image/Character/Enemy/") + imagePrefix + std::to_string(part) + ".png"
        );
        partImage->SetPosition(partPos);
        partImage->SetScale({1.75f, 1.75f});
        partImage->SetZIndex(5.0f);
        renderer.AddChild(partImage);
    }
}
}

Map::Map() 
    : m_Wall(RESOURCE_DIR "/Image/Background/Road/wall_b.bmp"),
      m_Floor(RESOURCE_DIR "/Image/Background/Road/road.bmp"),
      m_Lava1(RESOURCE_DIR "/Image/Background/Road/lava.bmp"),
      m_Lava2(RESOURCE_DIR "/Image/Background/Road/lava2.bmp"),
      m_Shine1(RESOURCE_DIR "/Image/Background/Road/wall_shine.bmp"),
      m_Shine2(RESOURCE_DIR "/Image/Background/Road/wall_shine2.bmp"),
      m_UpStairs(RESOURCE_DIR "/Image/Background/Stair/upstair.bmp"),   // 5
      m_DownStairs(RESOURCE_DIR "/Image/Background/Stair/downstair.bmp"), // 4
      m_YellowDoor(Door::DoorType::YELLOW),
      m_BlueDoor(Door::DoorType::BLUE),
      m_RedDoor(Door::DoorType::RED),
      m_GreenDoor(Door::DoorType::GREEN),
      m_IronFence(Door::DoorType::IRON),
      m_YellowKeyObj(Item::ItemType::YELLOWKEY),
      m_BlueKeyObj(Item::ItemType::BLUEKEY),
      m_RedKeyObj(Item::ItemType::REDKEY),
      m_CoinObj(Item::ItemType::COIN),
      m_RedPotionObj(Item::ItemType::RED_POTION),
      m_BluePotionObj(Item::ItemType::BLUE_POTION),
      m_RubyObj(Item::ItemType::RUBY),
      m_SapphireObj(Item::ItemType::SAPPHIRE),
      m_SwordEObj(Item::ItemType::SWORD_E),
      m_MagicKeyObj(Item::ItemType::MAGIC_KEY),
      m_ShieldCObj(Item::ItemType::SHIELD_C),
      m_SmallWingObj(Item::ItemType::SMALL_WING),
      m_CrossObj(Item::ItemType::CROSS),
      m_WindCompassObj(Item::ItemType::WIND_COMPASS),
      m_SwordCObj(Item::ItemType::SWORD_C),
      m_ShieldDObj(Item::ItemType::SHIELD_D),
      m_GemDiggerObj(Item::ItemType::GEM_DIGGER),
      m_BigWingObj(Item::ItemType::BIG_WING),
      m_HolyWaterObj(Item::ItemType::HOLY_WATER),
      m_SwordBObj(Item::ItemType::SWORD_B),
      m_ShieldAObj(Item::ItemType::SHIELD_A),
      m_RebVeriObj(Item::ItemType::RED_VERI),
      m_GreenVeriObj(Item::ItemType::GREEN_VERI),
      m_GodKnifeObj(Item::ItemType::GOD_KNIFE)

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
    m_YellowDoor.SetScale({newScale, newScale});
    m_BlueDoor.SetScale({newScale, newScale});
    m_RedDoor.SetScale({newScale, newScale});
    m_GreenDoor.SetScale({newScale, newScale});
    m_IronFence.SetScale({newScale, newScale});

    m_TileSize = 56.0f;

    auto loadFloorDataFiles = [&](const std::string& layerName,
                                  const std::string& itemName,
                                  const std::string& enemyName,
                                  const std::string& npcName,
                                  bool hasNPC = true) {
        LoadLevel(std::string(RESOURCE_DIR "/Layer/layer") + layerName + ".txt");
        LoadItems(std::string(RESOURCE_DIR "/Item/Item") + itemName + ".txt");
        LoadEnemies(std::string(RESOURCE_DIR "/Enemy/Enemy") + enemyName + ".txt");
        if (hasNPC) {
            LoadNPCs(std::string(RESOURCE_DIR "/NPC/NPC") + npcName + ".txt");
        } else {
            m_NPCRawData.push_back(std::vector<std::vector<int>>(11, std::vector<int>(11, 0)));
        }
    };

    auto loadFloorData = [&](const std::string& name, bool hasNPC = true) {
        loadFloorDataFiles(name, name, name, name, hasNPC);
    };

    for (int i = 0; i < 25; ++i) {
        loadFloorData(std::to_string(i));
    }
    loadFloorData("23_1", false);
    loadFloorData("23_3", false);

    m_CurrentLevel = 0;
    InitLevelEnemies();
    InitLevelNPCs();
}

void Map::Reset() {
    m_CurrentLevel = 0;
    m_MapData.clear();
    m_ItemData.clear();
    m_EnemyRawData.clear();
    m_NPCRawData.clear();
    m_Enemies.clear();
    m_NPCs.clear();
    m_NPCVisualParts.clear();

    m_AnimationTimer = 0.0f;
    m_ShowAltFrame = false;
    m_DoorAnimating = false;
    m_AnimatingDoorPos = {0.0f, 0.0f};
    m_AnimatingDoorType = 0;
    m_DoorReplacementTile = 0;
    m_FinalSealReleased = false;

    m_YellowDoor.Reset();
    m_BlueDoor.Reset();
    m_RedDoor.Reset();
    m_GreenDoor.Reset();
    m_IronFence.Reset();

    auto loadFloorDataFiles = [&](const std::string& layerName,
                                  const std::string& itemName,
                                  const std::string& enemyName,
                                  const std::string& npcName,
                                  bool hasNPC = true) {
        LoadLevel(std::string(RESOURCE_DIR "/Layer/layer") + layerName + ".txt");
        LoadItems(std::string(RESOURCE_DIR "/Item/Item") + itemName + ".txt");
        LoadEnemies(std::string(RESOURCE_DIR "/Enemy/Enemy") + enemyName + ".txt");
        if (hasNPC) {
            LoadNPCs(std::string(RESOURCE_DIR "/NPC/NPC") + npcName + ".txt");
        } else {
            m_NPCRawData.push_back(std::vector<std::vector<int>>(11, std::vector<int>(11, 0)));
        }
    };

    auto loadFloorData = [&](const std::string& name, bool hasNPC = true) {
        loadFloorDataFiles(name, name, name, name, hasNPC);
    };

    for (int i = 0; i < 25; ++i) {
        loadFloorData(std::to_string(i));
    }
    loadFloorData("23_1", false);
    loadFloorData("23_3", false);

    InitLevelEnemies();
    InitLevelNPCs();
}

void Map::UpdateAnimation(float deltaTime) {
    m_AnimationTimer += deltaTime;
    if (m_AnimationTimer >= 0.5f) {
        m_ShowAltFrame = !m_ShowAltFrame;
        m_AnimationTimer = 0.0f;
    }
}

void Map::LoadLevel(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        LOG_ERROR("Unable to open map floors : {}", filePath);
        return;
    }

    std::vector<std::vector<int>> tempLevel(11, std::vector<int>(11));
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {

            if (!(file >> tempLevel[i][j])) {
                LOG_ERROR("Map file format error: {}", filePath);

            }
        }
    }
    m_MapData.push_back(tempLevel);
    file.close();

    LOG_INFO("Floor successfully loaded: {}", filePath);
}

void Map::LoadItems(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        LOG_ERROR("Unable to open item save file: {}", filePath);
        return;
    }

    std::vector<std::vector<int>> tempItems(11, std::vector<int>(11, 0));
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            if (!(file >> tempItems[i][j])) {
                tempItems[i][j] = 0;
            }
        }
    }
    m_ItemData.push_back(tempItems);
    file.close();

    LOG_INFO("Item layer loaded successfully: {}", filePath);
}

void Map::LoadEnemies(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        LOG_ERROR("Unable to open enemy save file: {}", filePath);
        return;
    }

    std::vector<std::vector<int>> levelData(11, std::vector<int>(11));
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            file >> levelData[i][j];
        }
    }
    m_EnemyRawData.push_back(levelData);
    file.close();
    LOG_INFO("Enemy layer loaded successfully: {}", filePath);
}

void Map::LoadNPCs(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
       m_NPCRawData.push_back(std::vector<std::vector<int>>(11, std::vector<int>(11, 0)));
        LOG_ERROR("Unable to open NPC save file: {}", filePath);
        return;
    }


    std::vector<std::vector<int>> levelData(11, std::vector<int>(11));
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (!(file >> levelData[i][j])) {
               levelData[i][j] = 0;
            }
        }
    }


    m_NPCRawData.push_back(levelData);
    file.close();

    LOG_INFO("NPC layer loaded successfully: {}", filePath);
}

void Map::Draw() {
    if (m_MapData.empty() || m_ItemData.empty()) return;
    Util::Renderer renderer;

    float startX = -165.0f;
    float startY = 308.0f;

    auto& currentMap = m_MapData[m_CurrentLevel];
    auto& currentItems = m_ItemData[m_CurrentLevel]; // 取得物品層資料

    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            float posX = startX + (j * m_TileSize) + (m_TileSize / 2.0f);
            float posY = startY - (i * m_TileSize) - (m_TileSize / 2.0f);

            bool isThisDoorOpening = (m_DoorAnimating &&
                                      std::abs(posX - m_AnimatingDoorPos.x) < 0.1f &&
                                      std::abs(posY - m_AnimatingDoorPos.y) < 0.1f);

            if (isThisDoorOpening) {
                Door* animTarget = nullptr;
                if (m_AnimatingDoorType == 21) animTarget = &m_YellowDoor;
                else if (m_AnimatingDoorType == 22) animTarget = &m_BlueDoor;
                else if (m_AnimatingDoorType == 23) animTarget = &m_RedDoor;
                else if (m_AnimatingDoorType == 24) animTarget = &m_GreenDoor;
                else if (m_AnimatingDoorType == 25) animTarget = &m_IronFence;

                if (animTarget) {

                    animTarget->UpdateAnimation(Util::Time::GetDeltaTimeMs() / 1000.0f);

                    if (animTarget->IsFinished()) {
                        m_DoorAnimating = false;
                        RemoveTile(posX, posY);
                        animTarget->Reset();
                        currentItems[i][j] = 0;
                        currentMap[i][j] = m_DoorReplacementTile;
                        m_DoorReplacementTile = 0;
                    } else {
                        animTarget->SetPosition({posX, posY});
                        renderer.AddChild(std::shared_ptr<Util::GameObject>(std::shared_ptr<Util::GameObject>(), animTarget));
                        continue;
                    }
                }
            }


            m_Floor.SetPosition({posX, posY});
            m_Floor.SetZIndex(0.0f);
            renderer.AddChild(std::make_shared<BackgroundImage>(m_Floor));

            int tileType = currentMap[i][j];
            int itemType = currentItems[i][j];

            if (tileType != 0 && tileType != 1 && tileType != 2 && tileType != 3 && tileType != 4 && tileType != 5
                && tileType != 21 && tileType != 22 && tileType != 23 && tileType != 24 && tileType != 25) {
                LOG_DEBUG("The coordinates ({}, {}) read an unknown number: {}", i, j, tileType);
            }



            if (itemType != 0) {

                if (itemType == 10) {
                    m_YellowKeyObj.SetPosition({posX, posY});
                    m_YellowKeyObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_YellowKeyObj));
                }
                else if (itemType == 11) {
                    m_BlueKeyObj.SetPosition({posX, posY});
                    m_BlueKeyObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_BlueKeyObj));
                }
                else if (itemType == 12) {
                    m_RedKeyObj.SetPosition({posX, posY});
                    m_RedKeyObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_RedKeyObj));
                }
                else if (itemType == 30) {
                    m_CoinObj.SetPosition({posX, posY});
                    m_CoinObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_CoinObj));
                }
                else if (itemType == 31) {
                    m_RedPotionObj.SetPosition({posX, posY});
                    m_RedPotionObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_RedPotionObj));
                }
                else if (itemType == 32) {
                    m_BluePotionObj.SetPosition({posX, posY});
                    m_BluePotionObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_BluePotionObj));
                }
                else if (itemType == 33) {
                    m_RubyObj.SetPosition({posX, posY});
                    m_RubyObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_RubyObj));
                }
                else if (itemType == 34) {
                    m_SapphireObj.SetPosition({posX, posY});
                    m_SapphireObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_SapphireObj));
                }
                else if (itemType == 35) {
                    m_SwordEObj.SetPosition({posX, posY});
                    m_SwordEObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_SwordEObj));
                }
                else if (itemType == 36) {
                    m_MagicKeyObj.SetPosition({posX, posY});
                    m_MagicKeyObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_MagicKeyObj));
                }
                else if (itemType == 37) {
                    m_ShieldCObj.SetPosition({posX, posY});
                    m_ShieldCObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_ShieldCObj));
                }
                else if (itemType == 38) {
                    m_SmallWingObj.SetPosition({posX, posY});
                    m_SmallWingObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_SmallWingObj));
                }
                else if (itemType == 39) {
                    m_CrossObj.SetPosition({posX, posY});
                    m_CrossObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_CrossObj));
                }
                else if (itemType == 40) {
                    m_WindCompassObj.SetPosition({posX, posY});
                    m_WindCompassObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_WindCompassObj));
                }
                else if (itemType == 41) {
                    m_SwordCObj.SetPosition({posX, posY});
                    m_SwordCObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_SwordCObj));
                }
                else if (itemType == 42) {
                    m_ShieldDObj.SetPosition({posX, posY});
                    m_ShieldDObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_ShieldDObj));
                }
                else if (itemType == 43) {
                    m_GemDiggerObj.SetPosition({posX, posY});
                    m_GemDiggerObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_GemDiggerObj));
                }
                else if (itemType == 44) {
                    m_BigWingObj.SetPosition({posX, posY});
                    m_BigWingObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_BigWingObj));
                }
                else if (itemType == 45) {
                    m_HolyWaterObj.SetPosition({posX, posY});
                    m_HolyWaterObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_HolyWaterObj));
                }
                else if (itemType == 46) {
                    m_SwordBObj.SetPosition({posX, posY});
                    m_SwordBObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_SwordBObj));
                }
                else if (itemType == 47) {
                    m_ShieldAObj.SetPosition({posX, posY});
                    m_ShieldAObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_ShieldAObj));
                }
                else if (itemType == 48) {
                    m_RebVeriObj.SetPosition({posX, posY});
                    m_RebVeriObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_RebVeriObj));
                }
                else if (itemType == 49) {
                    m_GreenVeriObj.SetPosition({posX, posY});
                    m_GreenVeriObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_GreenVeriObj));
                }
                else if (itemType == 50) {
                    m_GodKnifeObj.SetPosition({posX, posY});
                    m_GodKnifeObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Item>(m_GodKnifeObj));
                }
            }
            else if (tileType != 0) {

                if (tileType == 1) {
                    m_Wall.SetPosition({posX, posY});
                    m_Wall.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<BackgroundImage>(m_Wall));
                }
                else if (tileType == 2) {
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
                else if (tileType == 3) {
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
                else if (tileType == 5) {
                    m_UpStairs.SetPosition({posX, posY});
                    m_UpStairs.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<BackgroundImage>(m_UpStairs));
                }
                else if (tileType == 4) {
                    m_DownStairs.SetPosition({posX, posY});
                    m_DownStairs.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<BackgroundImage>(m_DownStairs));
                }
                else if (tileType >= 21 && tileType <= 25) {

                    Door::DoorType type;
                    if (tileType == 21) type = Door::DoorType::YELLOW;
                    else if (tileType == 22) type = Door::DoorType::BLUE;
                    else if (tileType == 23) type = Door::DoorType::RED;
                    else if (tileType == 24) type = Door::DoorType::GREEN;
                    else type = Door::DoorType::IRON;


                    auto tempDoor = std::make_shared<Door>(type);
                    tempDoor->SetPosition({posX, posY});
                    tempDoor->SetZIndex(1.0f);

                    renderer.AddChild(tempDoor);
                }

                else {
                    m_Floor.SetPosition({posX, posY});
                    m_Floor.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<BackgroundImage>(m_Floor));
                }
            }
        }
    }

    for (auto& enemy : m_Enemies) {
        if (enemy->GetType() == Enemy::Type::OCTOPUS) {
            DrawLargeEnemy(renderer, *enemy, "Octopus", m_TileSize);
            continue;
        }
        if (enemy->GetType() == Enemy::Type::DRAGON) {
            DrawLargeEnemy(renderer, *enemy, "Drangon", m_TileSize);
            continue;
        }

        enemy->UpdateImage(m_ShowAltFrame);
        renderer.AddChild(enemy);
    }

    for (auto& npc : m_NPCs) {
        npc->UpdateImage(m_ShowAltFrame);
        renderer.AddChild(npc);
    }

    for (auto& part : m_NPCVisualParts) {
        const std::string& imagePath = m_ShowAltFrame ? part.imagePath2 : part.imagePath1;
        part.object->SetDrawable(std::make_shared<Util::Image>(imagePath));
        renderer.AddChild(part.object);
    }

    renderer.Update();
}

int Map::GetTileType(float x, float y) const {
    float startX = -165.0f;
    float startY = 308.0f;

    int col = static_cast<int>((x - startX + 0.1f) / m_TileSize);
    int row = static_cast<int>((startY - y + 0.1f) / m_TileSize);

    if (row < 0 || row >= 11 || col < 0 || col >= 11) {
        return -1;
    }

    if (m_ItemData[m_CurrentLevel][row][col] != 0) {
        return m_ItemData[m_CurrentLevel][row][col];
    }

    return m_MapData[m_CurrentLevel][row][col];
}

int Map::GetDisplayLevel() const {
    if (m_CurrentLevel == kLayer23_1Index || m_CurrentLevel == kLayer23_3Index) {
        return 23;
    }
    return m_CurrentLevel;
}

std::string Map::GetDisplayLevelName() const {
    if (m_CurrentLevel == 0) {
        return "序章";
    }
    if (m_CurrentLevel == 24) {
        return "地下層";
    }
    return "第" + std::to_string(GetDisplayLevel()) + "層";
}

glm::ivec2 Map::GetGridPosition(float x, float y) const {
    int col = static_cast<int>((x - m_StartX + 0.1f) / m_TileSize);
    int row = static_cast<int>((m_StartY - y + 0.1f) / m_TileSize);
    return {row, col};
}

glm::vec2 Map::GetTileCenter(int row, int col) const {
    float posX = m_StartX + (col * m_TileSize) + (m_TileSize / 2.0f);
    float posY = m_StartY - (row * m_TileSize) - (m_TileSize / 2.0f);
    return {posX, posY};
}

bool Map::IsWalkable(float x, float y) const {

    float startX = -165.0f;
    float startY = 308.0f;
    float mapSize = 11.0f * m_TileSize;

    if (x < startX || x >= (startX + mapSize) ||
        y > startY || y <= (startY - mapSize)) {
        return false;
        }

    int col = static_cast<int>((x - startX + 0.1f) / m_TileSize);
    int row = static_cast<int>((startY - y + 0.1f) / m_TileSize);


    if (row < 0 || row >= 11 || col < 0 || col >= 11) {
        return false;
    }


    int tileType = GetTileType(x, y);

    return (tileType == 0 || tileType == 4 || tileType == 5);
}

glm::vec2 Map::FindTilePosition(int targetType) const {
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
    return {0.0f, 0.0f};
}

void Map::RemoveTile(float x, float y) {
    float startX = -165.0f;
    float startY = 308.0f;

    int col = static_cast<int>((x - startX + 0.1f) / m_TileSize);
    int row = static_cast<int>((startY - y + 0.1f) / m_TileSize);

    if (row >= 0 && row < 11 && col >= 0 && col < 11) {

        m_ItemData[m_CurrentLevel][row][col] = 0;
        m_MapData[m_CurrentLevel][row][col] = 0;

    }
}

void Map::SetTileAtLevel(int level, int row, int col, int tileType) {
    if (level < 0 || level >= static_cast<int>(m_MapData.size())) return;
    if (row < 0 || row >= static_cast<int>(m_MapData[level].size())) return;
    if (col < 0 || col >= static_cast<int>(m_MapData[level][row].size())) return;

    m_MapData[level][row][col] = tileType;
}

void Map::RemoveTileAtLevel(int level, int row, int col) {
    SetTileAtLevel(level, row, col, 0);

    if (level < 0 || level >= static_cast<int>(m_ItemData.size())) return;
    if (row < 0 || row >= static_cast<int>(m_ItemData[level].size())) return;
    if (col < 0 || col >= static_cast<int>(m_ItemData[level][row].size())) return;

    m_ItemData[level][row][col] = 0;
}

void Map::InitLevelEnemies() {
    m_Enemies.clear();
    if (m_CurrentLevel >= static_cast<int>(m_EnemyRawData.size())) return;

    int level = m_CurrentLevel;
    for (int row = 0; row < 11; row++) {
        for (int col = 0; col < 11; col++) {
            int id = m_EnemyRawData[level][row][col];
            if (id <= 0) continue;


            std::shared_ptr<Enemy> enemy = nullptr;

            if (id == 50) enemy = std::make_shared<Enemy>(Enemy::Type::GREEN_SLIME);
            else if (id == 51) enemy = std::make_shared<Enemy>(Enemy::Type::RED_SLIME);
            else if (id == 52) enemy = std::make_shared<Enemy>(Enemy::Type::BAT);
            else if (id == 53) enemy =  std::make_shared<Enemy>(Enemy::Type::SKELETON_C);
            else if (id == 54) enemy =  std::make_shared<Enemy>(Enemy::Type::BIG_SLIME);
            else if (id == 55) enemy =  std::make_shared<Enemy>(Enemy::Type::SKELETON_B);
            else if (id == 56) enemy =  std::make_shared<Enemy>(Enemy::Type::PRIEST_C);
            else if (id == 57) enemy =  std::make_shared<Enemy>(Enemy::Type::BIG_BAT);
            else if (id == 58) enemy =  std::make_shared<Enemy>(Enemy::Type::ZOMBIE);
            else if (id == 59) enemy =  std::make_shared<Enemy>(Enemy::Type::SKELETON_A);
            else if (id == 60) enemy =  std::make_shared<Enemy>(Enemy::Type::ROCK);
            else if (id == 61) enemy =  std::make_shared<Enemy>(Enemy::Type::MAGICIAN_B);
            else if (id == 62) enemy =  std::make_shared<Enemy>(Enemy::Type::GATE_KEEPER_C);
            else if (id == 63) enemy =  std::make_shared<Enemy>(Enemy::Type::RED_BAT);
            else if (id == 64) enemy =  std::make_shared<Enemy>(Enemy::Type::MAGICIAN_A);
            else if (id == 65) enemy =  std::make_shared<Enemy>(Enemy::Type::SLIME_LORD);
            else if (id == 66) enemy =  std::make_shared<Enemy>(Enemy::Type::PRIEST_A);
            else if (id == 67) enemy =  std::make_shared<Enemy>(Enemy::Type::MAGIC_SERGEANT_D);
            else if (id == 68) enemy =  std::make_shared<Enemy>(Enemy::Type::ZOMBIE_KNIGHT);
            else if (id == 69) enemy =  std::make_shared<Enemy>(Enemy::Type::GATE_KEEPER_B);
            else if (id == 70) enemy =  std::make_shared<Enemy>(Enemy::Type::GATE_KEEPER_A);
            else if (id == 71) enemy =  std::make_shared<Enemy>(Enemy::Type::SWORDS_MAN);
            else if (id == 72) enemy =  std::make_shared<Enemy>(Enemy::Type::KNIGHT);
            else if (id == 73) enemy =  std::make_shared<Enemy>(Enemy::Type::IRON_KNIGHT_A);
            else if (id == 74) enemy =  std::make_shared<Enemy>(Enemy::Type::SOUL_SKELETON_KNIGHT);
            else if (id == 75) enemy =  std::make_shared<Enemy>(Enemy::Type::DARK_KNIGHT);
            else if (id == 76) enemy =  std::make_shared<Enemy>(Enemy::Type::DARK_MAGICIAN);
            else if (id == 77) enemy =  std::make_shared<Enemy>(Enemy::Type::SOUL_SKELETON);
            else if (id == 78) enemy =  std::make_shared<Enemy>(Enemy::Type::MAGIC_SERGEANT_A);
            else if (id == 79) enemy =  std::make_shared<Enemy>(Enemy::Type::SLIME_MAN);
            else if (id == 80) enemy =  std::make_shared<Enemy>(Enemy::Type::VAMPIRE);
            else if (id == 81) {
                enemy = std::make_shared<Enemy>(
                    m_FinalSealReleased ? Enemy::Type::OCTOPUS : Enemy::Type::DRAGON
                );
            }
            else if (id == 82) enemy =  std::make_shared<Enemy>(Enemy::Type::DRAGON);

            if (enemy) {
                enemy->ApplyLevelStats(GetDisplayLevel());
                float posX = m_StartX + (col * m_TileSize) + (m_TileSize / 2.0f);
                float posY = m_StartY - (row * m_TileSize) - (m_TileSize / 2.0f);
                enemy->SetPosition({posX, posY});
                m_Enemies.push_back(enemy);
            }
        }
    }
}

std::shared_ptr<Enemy> Map::GetEnemyAt(float x, float y) {
    for (auto& enemy : m_Enemies) {

        glm::vec2 pos = enemy->m_Transform.translation;


        if (std::abs(pos.x - x) < 5.0f && std::abs(pos.y - y) < 5.0f) {
            return enemy;
        }
    }
    return nullptr;
}

void Map::RemoveEnemy(std::shared_ptr<Enemy> enemy) {
    if (!enemy) return;

    // 1. 取得敵人在螢幕上的座標
    glm::vec2 pos = enemy->m_Transform.translation;

    // 2. 將螢幕座標轉換回地圖的 row 和 col (使用你已有的計算公式)
    int col = static_cast<int>((pos.x - m_StartX + 0.1f) / m_TileSize);
    int row = static_cast<int>((m_StartY - pos.y + 0.1f) / m_TileSize);

    // 3. 安全檢查並修改原始數據，確保下次 InitLevelEnemies 時不會再生成它
    if (row >= 0 && row < 11 && col >= 0 && col < 11) {
        m_EnemyRawData[m_CurrentLevel][row][col] = 0; // 將該格設為空
        LOG_INFO("Enemy data cleared for floor {}, pos ({}, {})", m_CurrentLevel, row, col);
    }

    // 4. 從當前顯示容器中移除
    m_Enemies.erase(
        std::remove(m_Enemies.begin(), m_Enemies.end(), enemy),
        m_Enemies.end()
    );
}

void Map::InitLevelNPCs() {
    m_NPCs.clear();
    m_NPCVisualParts.clear();
    if (m_CurrentLevel >= static_cast<int>(m_NPCRawData.size())) return;

    auto addNPCVisualPart = [&](float x, float y, const std::string& imagePath1, const std::string& imagePath2) {
        auto part = std::make_shared<BackgroundImage>(imagePath1);
        part->SetPosition({x, y});
        part->SetScale({1.75f, 1.75f});
        part->SetZIndex(5.0f);
        m_NPCVisualParts.push_back({part, imagePath1, imagePath2});
    };

    for (int row = 0; row < 11; row++) {
        for (int col = 0; col < 11; col++) {
            int id = m_NPCRawData[m_CurrentLevel][row][col];
            if (id <= 0) continue;

            std::shared_ptr<NPC> npc = nullptr;

            if (id == 90) npc = std::make_shared<NPC>(NPC::Type::FAIRY1);
            else if (id == 91) npc = std::make_shared<NPC>(NPC::Type::ELDER1);
            else if (id == 92) npc = std::make_shared<NPC>(NPC::Type::MERCHANT1);
            else if (id == 93) npc = std::make_shared<NPC>(NPC::Type::THIEF);
            else if (id == 94) npc = std::make_shared<NPC>(NPC::Type::PRINCESS);
            else if (id == 95) npc = std::make_shared<NPC>(NPC::Type::ELDER2);
            else if (id == 96) npc = std::make_shared<NPC>(NPC::Type::MERCHANT2);
            else if (id == 97) npc = std::make_shared<NPC>(NPC::Type::ELDER3);
            else if (id == 98) npc = std::make_shared<NPC>(NPC::Type::FAIRY2);
            else if (id == 100) npc = std::make_shared<NPC>(NPC::Type::SHOP_1);
            else if (id == 101) npc = std::make_shared<NPC>(NPC::Type::SHOP_2);
            else if (id == 102) npc = std::make_shared<NPC>(NPC::Type::ELDER_SHOP_1);
            else if (id == 103) npc = std::make_shared<NPC>(NPC::Type::ELDER_SHOP_2);
            else if (id == 104) npc = std::make_shared<NPC>(NPC::Type::MERCHANT_SHOP_1);
            else if (id == 105) npc = std::make_shared<NPC>(NPC::Type::MERCHANT_SHOP_2);

            if (npc) {
                float posX = m_StartX + (col * m_TileSize) + (m_TileSize / 2.0f);
                float posY = m_StartY - (row * m_TileSize) - (m_TileSize / 2.0f);
                npc->SetPosition({posX, posY});
                m_NPCs.push_back(npc);

                if (id == 100 || id == 101) {
                    const std::string prefix = RESOURCE_DIR "/Image/Character/Shop/Shop";

                    addNPCVisualPart(
                        posX - m_TileSize,
                        posY,
                        prefix + "A1.png",
                        prefix + "B1.png"
                    );
                    addNPCVisualPart(
                        posX + m_TileSize,
                        posY,
                        prefix + "A3.png",
                        prefix + "B3.png"
                    );
                }
            }
        }
    }
}

std::shared_ptr<NPC> Map::GetNPCAt(float x, float y) {
    for (auto& npc : m_NPCs) {
        // 獲取 NPC 的座標 (根據你之前的修正，使用 m_Transform.translation)
        glm::vec2 pos = npc->m_Transform.translation;

        // 判定玩家目標座標與 NPC 座標是否重疊 (容許誤差 5.0f)
        if (std::abs(pos.x - x) < 5.0f && std::abs(pos.y - y) < 5.0f) {
            return npc;
        }
    }
    return nullptr;
}

void Map::MoveNPC(std::shared_ptr<NPC> npc, int colOffset, int rowOffset) {
    if (!npc) return;

    // 1. 取得舊位置
    glm::vec2 oldPos = npc->m_Transform.translation;
    int oldCol = static_cast<int>((oldPos.x - m_StartX + 0.1f) / m_TileSize);
    int oldRow = static_cast<int>((m_StartY - oldPos.y + 0.1f) / m_TileSize);

    // 2. 計算新索引位置
    int newCol = oldCol + colOffset;
    int newRow = oldRow + rowOffset;

    // 3. 邊界檢查與原始數據更新
    if (newRow >= 0 && newRow < 11 && newCol >= 0 && newCol < 11) {
        // 將舊位置的 NPC 種類儲存起來，轉移到新位置
        int npcID = m_NPCRawData[m_CurrentLevel][oldRow][oldCol];
        m_NPCRawData[m_CurrentLevel][oldRow][oldCol] = 0; // 舊位置清空
        m_NPCRawData[m_CurrentLevel][newRow][newCol] = npcID; // 新位置填入

        // 4. 更新視覺座標
        float newPosX = m_StartX + (newCol * m_TileSize) + (m_TileSize / 2.0f);
        float newPosY = m_StartY - (newRow * m_TileSize) - (m_TileSize / 2.0f);
        npc->m_Transform.translation = {newPosX, newPosY};

        LOG_INFO("NPC moved to ({}, {})", newRow, newCol);
    }
}

void Map::RemoveNPC(std::shared_ptr<NPC> npc) {
    if (!npc) return;

    glm::vec2 pos = npc->m_Transform.translation;
    int col = static_cast<int>((pos.x - m_StartX + 0.1f) / m_TileSize);
    int row = static_cast<int>((m_StartY - pos.y + 0.1f) / m_TileSize);

    if (m_CurrentLevel >= 0 &&
        m_CurrentLevel < static_cast<int>(m_NPCRawData.size()) &&
        row >= 0 && row < 11 &&
        col >= 0 && col < 11) {
        m_NPCRawData[m_CurrentLevel][row][col] = 0;
    }

    m_NPCs.erase(
        std::remove(m_NPCs.begin(), m_NPCs.end(), npc),
        m_NPCs.end()
    );
}

