#include "Character/Player.hpp"
#include "Map/Map.hpp"
#include "Map/Stair.hpp"
#include "System/CollisionHandler.hpp"
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

    // 2. 將碰撞與觸發邏輯委託給 CollisionHandler
    // 傳入 *this 與 map，讓 Handler 處理數值增減與門的動畫觸發
    if (CollisionHandler::HandleCollision(*this, map, targetPos)) {
        // 如果 Handler 回傳 true，代表該格子可踏入（如地板、撿完的物品）
        m_Transform.translation = targetPos;

        // 3. 處理樓層傳送 (Stair 也是一種獨立的 System)
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