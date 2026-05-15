#include "NPC/NPC.hpp"
#include "Util/Image.hpp"

// 定義 NPC 數據表
const std::unordered_map<NPC::Type, NPC::Stats> NPC::s_NPCDataTable = {
    {NPC::Type::FAIRY1,   {"仙子",
        RESOURCE_DIR "/Image/Character/NPC/Fairy1.png",
        RESOURCE_DIR "/Image/Character/NPC/Fairy2.png"}},
    {NPC::Type::FAIRY2,   {"仙子",
        RESOURCE_DIR "/Image/Character/NPC/Fairy1.png",
        RESOURCE_DIR "/Image/Character/NPC/Fairy2.png"}},
    {NPC::Type::ELDER1,   {"長者",
        RESOURCE_DIR "/Image/Character/NPC/Elder1.png",
        RESOURCE_DIR "/Image/Character/NPC/Elder2.png"}},
    {NPC::Type::ELDER2,   {"長者",
        RESOURCE_DIR "/Image/Character/NPC/Elder1.png",
        RESOURCE_DIR "/Image/Character/NPC/Elder2.png"}},
    {NPC::Type::ELDER3,   {"長者",
        RESOURCE_DIR "/Image/Character/NPC/Elder1.png",
        RESOURCE_DIR "/Image/Character/NPC/Elder2.png"}},
    {NPC::Type::MERCHANT1,  {"商人",
        RESOURCE_DIR "/Image/Character/NPC/Merchant1.png", 
        RESOURCE_DIR "/Image/Character/NPC/Merchant2.png"}},
    {NPC::Type::MERCHANT2,  {"商人",
        RESOURCE_DIR "/Image/Character/NPC/Merchant1.png",
        RESOURCE_DIR "/Image/Character/NPC/Merchant2.png"}},
    {NPC::Type::THIEF,     {"小偷", 
        RESOURCE_DIR "/Image/Character/NPC/Thief1.png", 
        RESOURCE_DIR "/Image/Character/NPC/Thief2.png"}},
    {NPC::Type::PRINCESS,     {"公主",
        RESOURCE_DIR "/Image/Character/NPC/Princess1.png",
        RESOURCE_DIR "/Image/Character/NPC/Princess2.png"}},
    {NPC::Type::SHOP_1, {"商店",
        RESOURCE_DIR "/Image/Character/Shop/ShopA2.png",
        RESOURCE_DIR "/Image/Character/Shop/ShopB2.png"}},
    {NPC::Type::SHOP_2, {"商店",
        RESOURCE_DIR "/Image/Character/Shop/ShopA2.png",
        RESOURCE_DIR "/Image/Character/Shop/ShopB2.png"}},
    {NPC::Type::ELDER_SHOP_1, {"長者商店",
        RESOURCE_DIR "/Image/Character/Shop/Elder1.png",
        RESOURCE_DIR "/Image/Character/Shop/Elder2.png"}},
    {NPC::Type::ELDER_SHOP_2, {"長者商店",
        RESOURCE_DIR "/Image/Character/Shop/Elder1.png",
        RESOURCE_DIR "/Image/Character/Shop/Elder2.png"}},
    {NPC::Type::MERCHANT_SHOP_1, {"商人商店",
        RESOURCE_DIR "/Image/Character/Shop/ShopKeeper1.png",
        RESOURCE_DIR "/Image/Character/Shop/ShopKeeper2.png"}},
    {NPC::Type::MERCHANT_SHOP_2, {"商人商店",
        RESOURCE_DIR "/Image/Character/Shop/ShopKeeper1.png",
        RESOURCE_DIR "/Image/Character/Shop/ShopKeeper2.png"}}
};

NPC::NPC(Type type) 
    : BackgroundImage(s_NPCDataTable.at(type).imagePath1), m_Type(type) {
    
    m_Stats = s_NPCDataTable.at(type);
    
    this->SetScale({1.75f, 1.75f});

    this->m_ZIndex = 5.0f; 
}

void NPC::UpdateImage(bool showAltFrame) {
    // 檢查狀態是否改變，避免每一幀都重新載入圖片
    if (m_CurrentFrameIsAlt == showAltFrame) return;
    
    m_CurrentFrameIsAlt = showAltFrame;
    
    // 根據 PTSD 框架修正：更換 m_Drawable 內的影像資源
    if (showAltFrame) {
        this->m_Drawable = std::make_shared<Util::Image>(m_Stats.imagePath2);
    } else {
        this->m_Drawable = std::make_shared<Util::Image>(m_Stats.imagePath1);
    }
}
