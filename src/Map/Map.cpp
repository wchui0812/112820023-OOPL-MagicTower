#include "Map/Map.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

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
      m_DownStairs(RESOURCE_DIR "/Image/Background/Stair/downstair.bmp"), // 4
      m_YellowDoor(Door::DoorType::YELLOW),
      m_BlueDoor(Door::DoorType::BLUE),
      m_RedDoor(Door::DoorType::RED),
      m_GreenDoor(Door::DoorType::GREEN),
      m_IronFence(Door::DoorType::IRON),
      m_YellowKeyObj(Key::KeyType::YELLOW),
      m_BlueKeyObj(Key::KeyType::BLUE),
      m_RedKeyObj(Key::KeyType::RED),
      m_RedPotionObj(Item::ItemType::RED_POTION),
      m_BluePotionObj(Item::ItemType::BLUE_POTION),
      m_RubyObj(Item::ItemType::RUBY),
      m_SapphireObj(Item::ItemType::SAPPHIRE)

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

    for (int i = 0; i < 25; ++i) {
        // 讀取地圖層
        std::string mapPath = RESOURCE_DIR "/Layer/layer" + std::to_string(i) + ".txt";
        LoadLevel(mapPath);

        // 讀取物品層
        //std::string itemPath = RESOURCE_DIR "/Item/Item" + std::to_string(i) + ".txt";
        //LoadItems(itemPath);
    }
    for (int i = 0; i < 2; ++i) {
        // 讀取物品層
        std::string itemPath = RESOURCE_DIR "/Item/Item" + std::to_string(i) + ".txt";
        LoadItems(itemPath);
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
        LOG_ERROR("Unable to open map floors : {}", filePath);
        return;
    }

    std::vector<std::vector<int>> tempLevel(11, std::vector<int>(11));
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            // 直接將檔案中的數字讀入二維陣列
            if (!(file >> tempLevel[i][j])) {
                LOG_ERROR("Map file format error: {}", filePath);
                // 處理格式不符的情況
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
                tempItems[i][j] = 0; // 讀取失敗預設為空
            }
        }
    }
    m_ItemData.push_back(tempItems); // 加入對應樓層
    file.close();

    LOG_INFO("Item layer loaded successfully: {}", filePath);
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
                    // 只有這一格會觸發 UpdateAnimation
                    animTarget->UpdateAnimation(Util::Time::GetDeltaTimeMs() / 1000.0f);

                    if (animTarget->IsFinished()) {
                        m_DoorAnimating = false;
                        RemoveTile(posX, posY); // 播完後移除格子資料
                        animTarget->Reset();    // 重置回第一幀供下次使用
                    } else {
                        animTarget->SetPosition({posX, posY});
                        renderer.AddChild(std::shared_ptr<Util::GameObject>(std::shared_ptr<Util::GameObject>(), animTarget));
                        continue; // 畫完動畫門後跳過，不執行下方的一般繪製
                    }
                }
            }

            // --- 第一層：永遠繪製地板作為底色 ---
            m_Floor.SetPosition({posX, posY});
            m_Floor.SetZIndex(0.0f);
            renderer.AddChild(std::make_shared<BackgroundImage>(m_Floor));

            int tileType = currentMap[i][j];
            int itemType = currentItems[i][j];

            if (tileType != 0 && tileType != 1 && tileType != 2 && tileType != 3 && tileType != 4 && tileType != 5
                && tileType != 21 && tileType != 22 && tileType != 23 && tileType != 24 && tileType != 25) {
                LOG_DEBUG("The coordinates ({}, {}) read an unknown number: {}", i, j, tileType);
            }

            //BackgroundImage* target = nullptr; // 用指標暫存要畫的圖片

            // --- 第二層：優先判定物品層，若無物品則判定地形層 ---
            if (itemType != 0) {
                // 根據物品編號選取物件（從您的 Item 檔案路徑載入的物件）
                if (itemType == 10) {
                    m_YellowKeyObj.SetPosition({posX, posY});
                    m_YellowKeyObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Key>(m_YellowKeyObj));
                }
                else if (itemType == 11) {
                    m_BlueKeyObj.SetPosition({posX, posY});
                    m_BlueKeyObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Key>(m_BlueKeyObj));
                }
                else if (itemType == 12) {
                    m_RedKeyObj.SetPosition({posX, posY});
                    m_RedKeyObj.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<Key>(m_RedKeyObj));
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
            }
            else if (tileType != 0) {
                // 原有的地形判定
                if (tileType == 1) { // 牆壁
                    m_Wall.SetPosition({posX, posY});
                    m_Wall.SetZIndex(1.0f);
                    renderer.AddChild(std::make_shared<BackgroundImage>(m_Wall));
                }
                else if (tileType == 2) { // 岩漿
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
                else if (tileType == 3) { // 閃爍牆
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
                    // 【非動畫格】：建立一個「臨時物件」
                    // 這樣它永遠只會顯示第一影格，且不會被 animTarget->UpdateAnimation() 影響
                    Door::DoorType type;
                    if (tileType == 21) type = Door::DoorType::YELLOW;
                    else if (tileType == 22) type = Door::DoorType::BLUE;
                    else if (tileType == 23) type = Door::DoorType::RED;
                    else if (tileType == 24) type = Door::DoorType::GREEN;
                    else type = Door::DoorType::IRON;

                    // 建立臨時局部物件 tempDoor
                    auto tempDoor = std::make_shared<Door>(type);
                    tempDoor->SetPosition({posX, posY});
                    tempDoor->SetZIndex(1.0f);

                    // 將這個臨時物件交給渲染器
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
    renderer.Update();
}

int Map::GetTileType(float x, float y) const {
    float startX = -165.0f;
    float startY = 308.0f;

    // 換算成索引
    int col = static_cast<int>((x - startX + 0.1f) / m_TileSize);
    int row = static_cast<int>((startY - y + 0.1f) / m_TileSize);

    // 邊界安全檢查
    if (row < 0 || row >= 11 || col < 0 || col >= 11) {
        return -1; // 超出範圍
    }

    if (m_ItemData[m_CurrentLevel][row][col] != 0) {
        return m_ItemData[m_CurrentLevel][row][col];
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
    //int tileType = m_MapData[m_CurrentLevel][row][col];
    int tileType = GetTileType(x, y);

    // 4. 通行判斷：地板(0)、下樓梯(4)、上樓梯(5)
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
    return {0.0f, 0.0f}; // 若沒找到則回傳中心點
}

void Map::RemoveTile(float x, float y) {
    float startX = -165.0f;
    float startY = 308.0f;

    int col = static_cast<int>((x - startX + 0.1f) / m_TileSize);
    int row = static_cast<int>((startY - y + 0.1f) / m_TileSize);

    if (row >= 0 && row < 11 && col >= 0 && col < 11) {
        // 修改物品層資料，將其設為 0（空格）
        m_ItemData[m_CurrentLevel][row][col] = 0;
        m_MapData[m_CurrentLevel][row][col] = 0;
        LOG_INFO("Item removed, floor: {}, coordinates: ({}, {})", m_CurrentLevel, row, col);
    }
}