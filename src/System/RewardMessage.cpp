#include "System/RewardMessage.hpp"

#include "Util/TransformUtils.hpp"

RewardMessage::RewardMessage() {
    // 1. 載入橫幅專用的背景圖 (請確保路徑正確)
    m_Background = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Item/itemDialog.bmp");

    // 將圖片設為 GameObject 的 Drawable
    m_Drawable = m_Background;

    // 2. 設定文字樣式與初始位置 (ZIndex 需高於背景圖)
    // 參數: 字體大小, 初始文字, ZIndex
    m_Text = std::make_shared<TextObject>(24, " ", 61.0f);

    // 文字相對於橫幅中心的偏移 (0,0 代表在黑條正中央)
    m_Text->m_Transform.translation = {0.0f, 0.0f};

    // 3. 設定橫幅本身的位置與層級
    m_Transform.translation = {0.0f, 0.0f}; // 預設放在畫面中心
    m_ZIndex = 60.0f;                      // 確保高於戰鬥視窗 (50.0f)

    m_Visible = false;
    SetVisible(false);
}

void RewardMessage::ShowItem(const std::string& message) {
    m_Text->SetText(message);

    m_Visible = true;
    SetVisible(true);
    m_DisplayTimer = 0.0f; // 重設計時器讓它自動消失
}

void RewardMessage::Show(const std::string& message) {
    m_Text->SetText(message);
    m_Visible = true;
    SetVisible(true);
    m_DisplayTimer = 0.0f;
}

void RewardMessage::Update(float deltaTime) {
    if (!m_Visible) return;

    m_DisplayTimer += deltaTime;

    if (m_DisplayTimer >= m_MaxDisplayTime || Util::Input::IsKeyDown(Util::Keycode::Z)) {
        m_Visible = false;
        SetVisible(false);
    }
}

void RewardMessage::Draw() {
    if (!m_Visible) return;

    m_Background->Draw(Util::ConvertToUniformBufferData(m_Transform, m_Background->GetSize(), m_ZIndex));

    if (m_ItemIcon) {
        Util::Transform iconTransform;
        iconTransform.translation = m_Transform.translation + glm::vec2(-120.0f, 0.0f);
        iconTransform.scale = {1.0f, 1.0f};
        m_ItemIcon->Draw(Util::ConvertToUniformBufferData(iconTransform, m_ItemIcon->GetSize(), m_ZIndex + 1));
    }

    m_Text->Draw();
}