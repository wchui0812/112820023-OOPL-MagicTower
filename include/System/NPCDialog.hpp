#ifndef NPC_DIALOG_HPP
#define NPC_DIALOG_HPP

#include "System/TextObject.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

#include <memory>
#include <functional>
#include <string>
#include <vector>

class NPCDialog : public Util::GameObject {
public:
    struct DialogLine {
        std::string speakerName;
        std::string text;
        std::string portraitPath;
    };

    NPCDialog();

    void Start(const std::string& speakerName, const std::vector<std::string>& lines);
    void Start(const std::vector<DialogLine>& lines);
    void Start(const std::vector<DialogLine>& lines, std::function<void()> onFinish);
    void Update(float deltaTime);
    void Draw();

    bool IsActive() const { return m_Active; }

private:
    void RefreshText();
    void Finish();
    std::vector<std::string> WrapText(const std::string& text) const;
    void SetTextLeftAligned(TextObject& textObject, const std::string& text, const glm::vec2& leftCenterPos) const;

    bool m_Active = false;
    float m_InputCooldown = 0.0f;
    const float m_InputDelay = 0.18f;

    std::size_t m_CurrentLine = 0;
    std::string m_SpeakerName;
    std::vector<std::string> m_Lines;
    std::vector<DialogLine> m_DialogLines;
    std::function<void()> m_OnFinish;

    std::shared_ptr<Util::Image> m_Background;
    std::shared_ptr<Util::Image> m_Portrait;
    std::shared_ptr<TextObject> m_NameText;
    std::vector<std::shared_ptr<TextObject>> m_LineTexts;
    std::shared_ptr<TextObject> m_HintText;
};

#endif
