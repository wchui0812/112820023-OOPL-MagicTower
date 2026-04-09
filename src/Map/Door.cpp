#include "Map/Door.hpp"
#include "Util/Logger.hpp"

Door::Door(DoorType type) :
    BackgroundImage(
     type == DoorType::YELLOW ? RESOURCE_DIR "/Image/Door/yellow_door1.bmp" :
        type == DoorType::BLUE   ? RESOURCE_DIR "/Image/Door/blue_door1.bmp"   :
        type == DoorType::RED    ? RESOURCE_DIR "/Image/Door/red_door1.bmp"    :
        type == DoorType::GREEN  ? RESOURCE_DIR "/Image/Door/green_door1.bmp"  :
                                   RESOURCE_DIR "/Image/Door/iron_fence1.bmp"
    ) ,
    m_Type(type)
{
    std::string colorName;
    if (type == DoorType::YELLOW) colorName = "yellow";
    else if (type == DoorType::BLUE) colorName = "blue";
    else if (type == DoorType::RED) colorName = "red";
    else if (type == DoorType::GREEN) colorName = "green";

    // 載入 5 張動畫幀 (0 是閉合, 4 是全開)
    m_Frames.clear();
    for (int i = 1; i <= 5; ++i) {
        std::string path = RESOURCE_DIR "/Image/Door/" + colorName + "_door" + std::to_string(i) + ".bmp";
        m_Frames.push_back(std::make_shared<Util::Image>(path));
    }
    m_Drawable = m_Frames[0];
    this->SetScale({0.73f, 0.73f});
}

void Door::UpdateAnimation(float deltaTime) {
    if (!m_IsAnimating || m_IsFinished) return;

    m_Timer += deltaTime;
    if (m_Timer >= m_FrameInterval) {
        m_Timer = 0.0f;
        m_CurrentFrame++;
        if (static_cast<size_t>(m_CurrentFrame) < m_Frames.size()) {
            m_Drawable = m_Frames[m_CurrentFrame];
        } else {
            m_IsFinished = true;
        }
    }
}

void Door::Reset() {
    m_IsAnimating = false;
    m_IsFinished = false;
    m_CurrentFrame = 0;
    m_Timer = 0.0f;           // 建議同時重設計時器，確保下次動畫從頭開始
    if (!m_Frames.empty()) {
        m_Drawable = m_Frames[0]; // 強制換回第一張圖 (閉合狀態)
    }
}
