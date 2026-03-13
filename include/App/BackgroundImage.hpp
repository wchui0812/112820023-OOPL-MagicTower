#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include <string>
#include "Util/GameObject.hpp" // 引用 PTSD 框架的基礎物件類別
#include "Util/Image.hpp"      // 引用 PTSD 框架的圖片處理類別

class BackgroundImage : public Util::GameObject {
public:
    // 建構子：接收圖片路徑並載入
    explicit BackgroundImage(const std::string& path) 
        : m_Image(std::make_shared<Util::Image>(path)) {
        m_Drawable = m_Image; // 將圖片設為可繪製物件
    }

    // 設定圖片在螢幕上的座標 (x, y)
    void SetPosition(const glm::vec2& position) {
        m_Transform.translation = position;
    }

    // 執行繪製 (會由 App 的 Render 自動或手動呼叫)
    void Draw() {
        m_Image->Draw(m_Transform, m_ZIndex);
    }

private:
    std::shared_ptr<Util::Image> m_Image;
    float m_ZIndex = 0.0f; // 背景通常層級較低
};

#endif