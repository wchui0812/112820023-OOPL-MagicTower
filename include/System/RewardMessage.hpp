#ifndef MAGICTOWER_REWARDMESSAGE_HPP
#define MAGICTOWER_REWARDMESSAGE_HPP


#include "System/TextObject.hpp"

#include "Util/Input.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <memory>
#include <string>

class RewardMessage : public Util::GameObject {
public:
    RewardMessage();
    void Show(const std::string& message);
    void ShowItem(const std::string& message);
    void Update(float delta_time);
    void Draw();
    bool IsVisible() const {
        return m_Visible;
    }


private:
    std::shared_ptr<Util::Image> m_Background;
    std::shared_ptr<Util::Image> m_ItemIcon;
    std::shared_ptr<TextObject> m_Text;
    bool m_Visible = false;

    float m_DisplayTimer = 0.0f;
    const float m_MaxDisplayTime = 1.0f;
};

#endif //MAGICTOWER_REWARDMESSAGE_HPP