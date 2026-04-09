#include "Character/Player.hpp"
#include "Map/Map.hpp"
#include "Map/Stair.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

Player::Player() {
    std::map<Direction, std::string> dirCode = {
        {Direction::UP, "u"}, {Direction::DOWN, "d"},
        {Direction::LEFT, "tl"}, {Direction::RIGHT, "tr"}
    };

    // 預載圖片：RESOURCE_DIR "/Image/Character/Player/player_u1.png"
    for (auto const& [dir, code] : dirCode) {
        for (int i = 1; i <= 3; ++i) {
            std::string path = RESOURCE_DIR "/Image/Character/Player/player_" + code + std::to_string(i) + ".png";
            m_AnimationFrames[dir].push_back(std::make_shared<Util::Image>(path));
        }
    }

    m_Drawable = m_AnimationFrames[m_CurrentDir][0];
    m_ZIndex = 5.0f;
}

void Player::Update(Map& map) {
    float moveDist = 56.0f;
    glm::vec2 currentPos = m_Transform.translation;
    glm::vec2 targetPos = currentPos; // 用來存放「預計要去的那一格」
    glm::vec2 moveVec = {0, 0};
    bool moved = false;

    // 1. 根據輸入決定「目標格子」 targetPos
    if (Util::Input::IsKeyDown(Util::Keycode::UP)) {
        SetDirection(Direction::UP);
        targetPos.y += moveDist;
        moveVec = {0, 1};
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
        SetDirection(Direction::DOWN);
        targetPos.y -= moveDist;
        moveVec = {0, -1};
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::LEFT)) {
        SetDirection(Direction::LEFT);
        targetPos.x -= moveDist;
        moveVec = {-1, 0};
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
        SetDirection(Direction::RIGHT);
        targetPos.x += moveDist;
        moveVec = {1, 0};
    }

    // 如果沒有按下任何鍵，直接結束
    if (targetPos == currentPos) return;

    // 2. 取得目標格子的編號 (包含物品層與地形層)
    int tileType = map.GetTileType(targetPos.x, targetPos.y);

    // 3. 處理撿起物品 (10-12)
    if (tileType >= 10 && tileType <= 12) {
        if (tileType == 10) m_YellowKeys++;
        else if (tileType == 11) m_BlueKeys++;
        else if (tileType == 12) m_RedKeys++;

        map.RemoveTile(targetPos.x, targetPos.y);
        m_Transform.translation = targetPos; // 踏上去
        moved = true;
    }

    else if (tileType >= 31 && tileType <= 34) {
        if (tileType == 31) m_Hp += 200;  // 紅血瓶
        else if (tileType == 32) m_Hp += 500;  // 藍血瓶
        else if (tileType == 33) m_Atk += 3;   // 紅寶石
        else if (tileType == 34) m_Def += 3;   // 藍寶石

        map.RemoveTile(targetPos.x, targetPos.y);
        m_Transform.translation = targetPos;
        moved = true;
        LOG_INFO("Item acquired, current status - HP: {}, ATK: {}, DEF: {}", m_Hp, m_Atk, m_Def);
    }

    // 4. 處理開門 (21-25)
    else if (tileType >= 21 && tileType <= 25) {
        bool opened = false;
        if (tileType == 21 && m_YellowKeys > 0) { m_YellowKeys--; opened = true; }
        else if (tileType == 22 && m_BlueKeys > 0) { m_BlueKeys--; opened = true; }
        else if (tileType == 23 && m_RedKeys > 0) { m_RedKeys--; opened = true; }
        // 綠門與鐵門...

        if (opened) {
            //map.RemoveTile(targetPos.x, targetPos.y);
            map.TriggerDoorAnimation(targetPos.x, targetPos.y, tileType); // <-- 改成這行
            LOG_INFO("The door is open.");
            // 開門後玩家通常保持原位，下一格再走過去；或者直接踏上去：
            // m_Transform.translation = targetPos;
            // moved = true;
        }
    }
    // 5. 處理普通行走 (0, 4, 5)
    else if (map.IsWalkable(targetPos.x, targetPos.y)) {
        m_Transform.translation = targetPos;
        moved = true;
    }

    // 6. 處理樓層傳送
    if (moved) {
        Stair::CheckAndTransport(m_Transform.translation, map, moveVec);
    }
}

void Player::SetDirection(Direction dir) {
    if (m_CurrentDir != dir) {
        m_CurrentDir = dir;
        m_FrameIndex = 0; // 轉向時重設動畫到第一幀
    } else {
        NextFrame(); // 同方向移動時切換下一幀
    }
    m_Drawable = m_AnimationFrames[m_CurrentDir][m_FrameIndex];
}

void Player::NextFrame() {
    m_FrameIndex = (m_FrameIndex + 1) % 3;
    m_Drawable = m_AnimationFrames[m_CurrentDir][m_FrameIndex];
}

void Player::Move(const glm::vec2& delta) {
    m_Transform.translation += delta;
}