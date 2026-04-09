#ifndef DOOR_HPP
#define DOOR_HPP

#include "App/BackgroundImage.hpp"
#include "Util/Image.hpp"
#include <vector>

class Door : public BackgroundImage {
public:
    enum class DoorType { YELLOW, BLUE, RED, GREEN, IRON };

    Door(DoorType type);

    void StartAnimation() {
        m_IsAnimating = true;
        m_IsFinished = false;
        m_CurrentFrame = 0; // 從第 2 張圖開始播放
        m_Timer = 0.0f;
    }
    void UpdateAnimation(float deltaTime);
    bool IsAnimating() const { return m_IsAnimating; }
    bool IsFinished() const { return m_IsFinished; }
    void Reset();
private:
    DoorType m_Type;
    std::vector<std::shared_ptr<Util::Image>> m_Frames;
    int m_CurrentFrame = 0;
    float m_Timer = 0.0f;
    bool m_IsAnimating = false;
    bool m_IsFinished = false;
    const float m_FrameInterval = 0.1f; // 每幀間隔秒數
};

#endif