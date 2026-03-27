#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include <string>
#include <memory>
#include "Util/GameObject.hpp" // 引用 PTSD 框架的基礎物件類別
#include "Util/Image.hpp"      // 引用 PTSD 框架的圖片處理類別

class BackgroundImage : public Util::GameObject {
public:
    // 建構子：接收圖片路徑並載入
    explicit BackgroundImage(const std::string& path) {
        m_Drawable = std::make_shared<Util::Image>(path);
    }


    [[nodiscard]] std::shared_ptr<Core::Drawable> GetDrawable() const {
        return m_Drawable;
    }

    void SetScale(const glm::vec2& scale) {
        m_Transform.scale = scale;
    }

    // 設定圖片在螢幕上的座標 (x, y)
    void SetPosition(const glm::vec2& position) {
        m_Transform.translation = position;
    }

    //[[nodiscard]] std::shared_ptr<Util::Drawable> GetDrawable() const {
        //return m_Drawable;
    //}

    // 執行繪製 (會由 App 的 Render 自動或手動呼叫)
};

#endif