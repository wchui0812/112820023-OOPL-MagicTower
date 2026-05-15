#include "System/NPCDialog.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/TransformUtils.hpp"

#include <utility>

namespace {
std::size_t GetUtf8CharLength(unsigned char c) {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}
}

NPCDialog::NPCDialog() {
    m_Background = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/NPC/NPCDialog.bmp");
    m_Drawable = m_Background;

    m_ZIndex = 70.0f;
    m_Transform.translation = {0.0f, 0.0f};

    m_NameText = std::make_shared<TextObject>(24, " ", m_ZIndex + 1.0f);
    m_HintText = std::make_shared<TextObject>(20, "Space / Enter", m_ZIndex + 1.0f);
    for (int i = 0; i < 4; i++) {
        m_LineTexts.push_back(std::make_shared<TextObject>(26, " ", m_ZIndex + 1.0f));
    }

    m_NameText->m_Transform.translation = m_Transform.translation + glm::vec2(-223.0f, 10.0f);
    m_HintText->m_Transform.translation = m_Transform.translation + glm::vec2(205.0f, -120.0f);

    SetVisible(false);
}

void NPCDialog::Start(const std::string& speakerName, const std::vector<std::string>& lines) {
    if (lines.empty()) return;

    std::vector<DialogLine> dialogLines;
    dialogLines.reserve(lines.size());
    for (const auto& line : lines) {
        dialogLines.push_back({speakerName, line, " "});
    }

    Start(dialogLines, nullptr);
}

void NPCDialog::Start(const std::vector<DialogLine>& lines) {
    Start(lines, nullptr);
}

void NPCDialog::Start(const std::vector<DialogLine>& lines, std::function<void()> onFinish) {
    if (lines.empty()) return;

    m_DialogLines = lines;
    m_OnFinish = std::move(onFinish);
    m_CurrentLine = 0;
    m_InputCooldown = m_InputDelay;
    m_Active = true;
    SetVisible(true);

    RefreshText();
}

void NPCDialog::Update(float deltaTime) {
    if (!m_Active) return;

    if (m_InputCooldown > 0.0f) {
        m_InputCooldown -= deltaTime;
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::SPACE) ||
        Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
        m_InputCooldown = m_InputDelay;
        m_CurrentLine++;

        if (m_CurrentLine >= m_DialogLines.size()) {
            Finish();
            return;
        }

        RefreshText();
    }
}

void NPCDialog::Draw() {
    if (!m_Active) return;

    m_Background->Draw(Util::ConvertToUniformBufferData(
        m_Transform,
        m_Background->GetSize(),
        m_ZIndex
    ));

    if (m_Portrait) {
        Util::Transform portraitTransform;
        portraitTransform.translation = m_Transform.translation + glm::vec2(-230.0f, 77.0f);
        portraitTransform.scale = {2.1f, 2.1f};
        m_Portrait->Draw(Util::ConvertToUniformBufferData(
            portraitTransform,
            m_Portrait->GetSize(),
            m_ZIndex + 1.0f
        ));
    }

    m_NameText->Draw();
    for (auto& lineText : m_LineTexts) {
        lineText->Draw();
    }
    m_HintText->Draw();
}

void NPCDialog::RefreshText() {
    const auto& line = m_DialogLines[m_CurrentLine];
    m_NameText->SetText(line.speakerName);

    const auto wrappedLines = WrapText(line.text);
    const glm::vec2 firstLineLeft = m_Transform.translation + glm::vec2(-155.0f, 75.0f);
    const float lineSpacing = 42.0f;
    for (std::size_t i = 0; i < m_LineTexts.size(); i++) {
        const std::string text = i < wrappedLines.size() ? wrappedLines[i] : " ";
        SetTextLeftAligned(
            *m_LineTexts[i],
            text,
            firstLineLeft + glm::vec2(0.0f, -lineSpacing * static_cast<float>(i))
        );
    }

    if (!line.portraitPath.empty()) {
        m_Portrait = std::make_shared<Util::Image>(line.portraitPath);
    } else {
        m_Portrait = nullptr;
    }
}

std::vector<std::string> NPCDialog::WrapText(const std::string& text) const {
    constexpr int maxLineUnits = 30;
    constexpr int maxLines = 4;

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

void NPCDialog::SetTextLeftAligned(
    TextObject& textObject,
    const std::string& text,
    const glm::vec2& leftCenterPos
) const {
    textObject.SetText(text);
    textObject.m_Transform.translation = leftCenterPos + glm::vec2(textObject.GetSize().x / 2.0f, 0.0f);
}

void NPCDialog::Finish() {
    m_Active = false;
    m_DialogLines.clear();
    m_Portrait = nullptr;
    SetVisible(false);

    if (m_OnFinish) {
        auto onFinish = std::move(m_OnFinish);
        m_OnFinish = nullptr;
        onFinish();
    }
}
