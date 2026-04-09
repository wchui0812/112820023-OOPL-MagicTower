#include "Item/Key.hpp"

Key::Key(KeyType type) : m_Type(type), 
    BackgroundImage(
     type == KeyType::YELLOW ? RESOURCE_DIR "/Image/Item/yellow_key.bmp" :
        type == KeyType::BLUE   ? RESOURCE_DIR "/Image/Item/blue_key.bmp"   :
                                  RESOURCE_DIR "/Image/Item/red_key.bmp"
    ) 
{
    this->SetScale({0.73f, 0.73f}); // 保持與地圖格子一致
}