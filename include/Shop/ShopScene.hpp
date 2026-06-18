#ifndef SHOP_SCENE_HPP
#define SHOP_SCENE_HPP

#include "System/TextObject.hpp"
#include "Shop/Shop.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

#include <memory>
#include <string>
#include <vector>

class Player;

class ShopScene : public Util::GameObject {
public:
    ShopScene();

    void Open(Player& player, const Shop& shop);
    void Update(float deltaTime);
    void Draw();

    bool IsActive() const { return m_Active; }

private:
    void MoveSelection(int delta);
    void ConfirmSelection();
    void RefreshTexts();
    void SetMessage(const std::string& message);
    std::vector<std::string> WrapText(const std::string& text, int maxLineUnits, int maxLines) const;
    void SetTextLeftAligned(TextObject& textObject, const std::string& text, const glm::vec2& leftCenterPos) const;
    void SetTextCentered(TextObject& textObject, const std::string& text, const glm::vec2& centerPos) const;

    bool m_Active = false;
    float m_InputCooldown = 0.0f;
    const float m_InputDelay = 0.18f;
    float m_PortraitTimer = 0.0f;
    const float m_PortraitFrameInterval = 0.5f;
    bool m_ShowAltPortrait = false;

    Player* m_Player = nullptr;
    int m_SelectedIndex = 0;

    std::shared_ptr<Util::Image> m_Background;
    std::shared_ptr<Util::Image> m_Portrait;
    std::shared_ptr<TextObject> m_TitleText;
    std::vector<std::shared_ptr<TextObject>> m_MessageTexts;
    std::shared_ptr<TextObject> m_HintText;
    std::vector<std::shared_ptr<TextObject>> m_OptionTexts;

    Shop m_Shop;
};

#endif
