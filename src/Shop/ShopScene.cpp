#include "Shop/ShopScene.hpp"
#include "Character/Player.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/TransformUtils.hpp"

namespace {
constexpr float kMapCenterX = -165.0f + (56.0f * 11.0f / 2.0f);
constexpr float kMapCenterY = 308.0f - (56.0f * 11.0f / 2.0f);

std::size_t GetUtf8CharLength(unsigned char c) {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}
}

ShopScene::ShopScene() {
    m_Background = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Character/Shop/ShopDialog.bmp");
    m_Portrait = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Character/Shop/ShopKeeper1.png");
    m_Drawable = m_Background;

    m_ZIndex = 75.0f;
    m_Transform.translation = {kMapCenterX, kMapCenterY};

    m_TitleText = std::make_shared<TextObject>(24, "商店", m_ZIndex + 1.0f);
    m_HintText = std::make_shared<TextObject>(18, "Up / Down   Space / Enter", m_ZIndex + 1.0f);

    for (int i = 0; i < 3; i++) {
        m_MessageTexts.push_back(std::make_shared<TextObject>(22, " ", m_ZIndex + 1.0f));
    }

    for (int i = 0; i < 4; i++) {
        m_OptionTexts.push_back(std::make_shared<TextObject>(24, " ", m_ZIndex + 1.0f));
    }

    SetVisible(false);
}

void ShopScene::Open(Player& player, const Shop& shop) {
    m_Player = &player;
    m_Shop = shop;
    m_ShowAltPortrait = false;
    m_PortraitTimer = 0.0f;
    if (!m_Shop.GetPortraitPath1().empty()) {
        m_Portrait = std::make_shared<Util::Image>(m_Shop.GetPortraitPath1());
    }
    m_SelectedIndex = 0;
    m_InputCooldown = m_InputDelay;
    m_Active = true;
    SetVisible(true);
    RefreshTexts();
}

void ShopScene::Update(float deltaTime) {
    if (!m_Active || !m_Player) return;

    m_PortraitTimer += deltaTime;
    if (m_PortraitTimer >= m_PortraitFrameInterval) {
        m_PortraitTimer = 0.0f;
        m_ShowAltPortrait = !m_ShowAltPortrait;
        const auto& portraitPath = m_ShowAltPortrait
            ? m_Shop.GetPortraitPath2()
            : m_Shop.GetPortraitPath1();
        if (!portraitPath.empty()) {
            m_Portrait = std::make_shared<Util::Image>(portraitPath);
        }
    }

    if (m_InputCooldown > 0.0f) {
        m_InputCooldown -= deltaTime;
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::UP)) {
        MoveSelection(-1);
    } else if (Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
        MoveSelection(1);
    } else if (Util::Input::IsKeyDown(Util::Keycode::SPACE) ||
               Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
        ConfirmSelection();
    }
}

void ShopScene::Draw() {
    if (!m_Active) return;

    m_Background->Draw(Util::ConvertToUniformBufferData(
        m_Transform,
        m_Background->GetSize(),
        m_ZIndex
    ));

    Util::Transform portraitTransform;
    portraitTransform.translation = m_Transform.translation + glm::vec2(-153.5f, 193.0f);
    portraitTransform.scale = {2.1f, 2.1f};
    m_Portrait->Draw(Util::ConvertToUniformBufferData(
        portraitTransform,
        m_Portrait->GetSize(),
        m_ZIndex + 1.0f
    ));

    m_TitleText->Draw();
    for (auto& messageText : m_MessageTexts) {
        messageText->Draw();
    }
    for (auto& optionText : m_OptionTexts) {
        optionText->Draw();
    }
    m_HintText->Draw();
}

void ShopScene::MoveSelection(int delta) {
    m_InputCooldown = m_InputDelay;
    const int optionCount = static_cast<int>(m_Shop.GetItems().size());
    if (optionCount == 0) return;
    m_SelectedIndex = (m_SelectedIndex + delta + optionCount) % optionCount;
    RefreshTexts();
}

void ShopScene::ConfirmSelection() {
    m_InputCooldown = m_InputDelay;

    const auto result = m_Shop.Purchase(*m_Player, m_SelectedIndex);
    if (result.message.empty() && !result.shouldClose) return;
    if (result.shouldClose) {
        m_Active = false;
        SetVisible(false);
        return;
    }

    SetMessage(result.message);
}

void ShopScene::RefreshTexts() {
    m_TitleText->SetText(m_Shop.GetTitle());
    m_TitleText->m_Transform.translation = m_Transform.translation + glm::vec2(-147.5f, 105.0f);
    SetMessage(m_Shop.GetGreeting());
    m_HintText->m_Transform.translation = m_Transform.translation + glm::vec2(140.0f, -280.0f);

    const auto& items = m_Shop.GetItems();
    for (std::size_t i = 0; i < m_OptionTexts.size(); i++) {
        if (i >= items.size()) {
            m_OptionTexts[i]->SetText(" ");
            continue;
        }
        const std::string prefix = static_cast<int>(i) == m_SelectedIndex ? "> " : "  ";
        SetTextCentered(
            *m_OptionTexts[i],
            prefix + items[i].text,
            m_Transform.translation + glm::vec2(0.0f, -50.0f - static_cast<float>(i) * 43.0f)
        );
    }
}

void ShopScene::SetMessage(const std::string& message) {
    const auto lines = WrapText(message, 24, static_cast<int>(m_MessageTexts.size()));
    const glm::vec2 firstLineLeft = m_Transform.translation + glm::vec2(-80.0f, 220.0f);
    const float lineSpacing = 32.0f;

    for (std::size_t i = 0; i < m_MessageTexts.size(); i++) {
        const std::string text = i < lines.size() ? lines[i] : " ";
        SetTextLeftAligned(
            *m_MessageTexts[i],
            text,
            firstLineLeft + glm::vec2(0.0f, -lineSpacing * static_cast<float>(i))
        );
    }
}

std::vector<std::string> ShopScene::WrapText(const std::string& text, int maxLineUnits, int maxLines) const {
    std::vector<std::string> lines;
    std::string currentLine;
    int currentUnits = 0;

    for (std::size_t i = 0; i < text.size();) {
        const std::size_t charLength = GetUtf8CharLength(static_cast<unsigned char>(text[i]));
        std::string token = text.substr(i, charLength);
        i += charLength;

        const int tokenUnits = charLength == 1 ? 1 : 2;
        if (!currentLine.empty() && currentUnits + tokenUnits > maxLineUnits) {
            lines.push_back(currentLine);
            currentLine.clear();
            currentUnits = 0;

            if (static_cast<int>(lines.size()) >= maxLines) break;
        }

        currentLine += token;
        currentUnits += tokenUnits;
    }

    if (!currentLine.empty() && static_cast<int>(lines.size()) < maxLines) {
        lines.push_back(currentLine);
    }

    if (lines.empty()) {
        lines.push_back(" ");
    }

    return lines;
}

void ShopScene::SetTextLeftAligned(
    TextObject& textObject,
    const std::string& text,
    const glm::vec2& leftCenterPos
) const {
    textObject.SetText(text);
    textObject.m_Transform.translation = leftCenterPos + glm::vec2(textObject.GetSize().x / 2.0f, 0.0f);
}

void ShopScene::SetTextCentered(
    TextObject& textObject,
    const std::string& text,
    const glm::vec2& centerPos
) const {
    textObject.SetText(text);
    textObject.m_Transform.translation = centerPos;
}
