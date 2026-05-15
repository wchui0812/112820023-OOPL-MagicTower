#ifndef NPC_HPP
#define NPC_HPP

#include "App/BackgroundImage.hpp"
#include <string>
#include <unordered_map>

class NPC : public BackgroundImage {
public:
    enum class Type {
        FAIRY1,
        FAIRY2,
        ELDER1,
        ELDER2,
        ELDER3,// 智者（老人）
        MERCHANT1,
        MERCHANT2,// 商人
        THIEF,      // 小偷
        PRINCESS,
        SHOP_1,
        SHOP_2,
        ELDER_SHOP_1,
        ELDER_SHOP_2,
        MERCHANT_SHOP_1,
        MERCHANT_SHOP_2
    };

    struct Stats {
        std::string name;
        std::string imagePath1;
        std::string imagePath2;
    };

    NPC(Type type);
    void UpdateImage(bool showAltFrame);

    Type GetType() const {
        return m_Type;
    }

private:
    Type m_Type;
    Stats m_Stats;
    bool m_CurrentFrameIsAlt = false;
    static const std::unordered_map<Type, Stats> s_NPCDataTable;
};

#endif
