#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>
#include <string>
#include <vector>

#include "Map/Map.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Player : public Util::GameObject {
public:
    enum class Direction { UP, DOWN, LEFT, RIGHT };

    explicit Player(); // 建構子改到 .cpp 實作

    void Update();

    void SetDirection(Direction dir);
    void Move(const glm::vec2& delta);
    
    // 工具函式
    void SetScale(const glm::vec2& scale) {
        m_Transform.scale = scale;
    }

    void SetPosition(const glm::vec2& position) {
        m_Transform.translation = position;
    }

    [[nodiscard]] const glm::vec2& GetPosition() const {
        return m_Transform.translation;
    }

    void Update(Map & map);

private:
    void NextFrame();

    Direction m_CurrentDir = Direction::DOWN;
    int m_FrameIndex = 0;
    float m_MoveDist = 56.0f;

    // 儲存所有預載圖片的容器
    std::map<Direction, std::vector<std::shared_ptr<Util::Image>>> m_AnimationFrames;
};

#endif