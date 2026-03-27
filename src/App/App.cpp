#include "App/App.hpp"
#include "Character/Player.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "Util/Time.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_MainBackground = std::make_shared<BackgroundImage>(RESOURCE_DIR "/Image/Scene/scene1.BMP");
    m_MainBackground->SetPosition({0, 0});
    m_MainBackground->SetZIndex(-1.0f); // 置於最底層

    // 1. 定義地圖對齊參數（必須與 Map::Draw 一致）
    float startX = -165.0f;
    float startY = 308.0f;
    float tileSize = 56.0f;

    // 2. 假設你想讓玩家出現在地圖的第 10 列、第 5 行（靠下方中間）
    int spawnRow = 10;
    int spawnCol = 5;

    // 3. 計算螢幕座標
    float playerX = startX + (spawnCol * tileSize) + (tileSize / 2.0f);
    float playerY = startY - (spawnRow * tileSize) - (tileSize / 2.0f);

    // 4. 設定玩家位置
    m_Player = std::make_shared<Player>();
    m_Player->SetPosition({playerX, playerY});
    m_Player->SetScale({1.6f, 1.6f});

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    static Util::Renderer renderer;

    m_GameMap.UpdateAnimation(Util::Time::GetDeltaTime());

    renderer.AddChild(m_MainBackground);
    m_GameMap.Draw();

    m_Player->Update(m_GameMap);
    renderer.AddChild(m_Player);

    renderer.Update();

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
