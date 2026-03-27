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
    glm::vec2 nextPos = m_Transform.translation;
    glm::vec2 moveVec = {0, 0}; // 新增：用來記錄移動方向
    bool moved = false;

    if (Util::Input::IsKeyDown(Util::Keycode::UP)) {
        SetDirection(Direction::UP);
        if (map.IsWalkable(nextPos.x, nextPos.y + moveDist)) {
            nextPos.y += moveDist;
            moveVec = {0, 1}; // 向上
            moved = true;
        }
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
        SetDirection(Direction::DOWN);
        float nextY = nextPos.y - moveDist;
        if (nextY >= -330.0f && map.IsWalkable(nextPos.x, nextY)) {
            nextPos.y -= moveDist;
            moveVec = {0, -1}; // 向下
            moved = true;
        }
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::LEFT)) {
        SetDirection(Direction::LEFT);
        if (map.IsWalkable(nextPos.x - moveDist, nextPos.y)) {
            nextPos.x -= moveDist;
            moveVec = {-1, 0}; // 向左
            moved = true;
        }
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
        SetDirection(Direction::RIGHT);
        if (map.IsWalkable(nextPos.x + moveDist, nextPos.y)) {
            nextPos.x += moveDist;
            moveVec = {1, 0}; // 向右
            moved = true;
        }
    }

    if (moved) {
        m_Transform.translation = nextPos;
        // 傳入 moveVec，讓 Stair 知道切換樓層後要往哪邊推
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