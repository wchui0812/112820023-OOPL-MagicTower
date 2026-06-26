#include "App/App.hpp"
#include "Map/Map.hpp"
#include "Character/Player.hpp"
#include "System/UIStatus.hpp"
#include "Enemy/Enemy.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "Util/Time.hpp"

#include <algorithm>

void App::Start() {
    LOG_TRACE("Start");

    m_MainBackground = std::make_shared<BackgroundImage>(RESOURCE_DIR "/Image/Scene/scene_final.BMP");
    m_MainBackground->SetPosition({0, 0});
    m_MainBackground->SetZIndex(-1.0f);

    const float startX = -165.0f;
    const float startY = 308.0f;
    const float tileSize = 56.0f;

    int spawnRow = 9;
    int spawnCol = 5;

    // ==================== Debug start settings ====================
    // kDebugMode:
    //   true  = skip login/loading/intro and start the game immediately.
    //   false = normal flow: login -> loading -> intro -> game.
    //
    // To test another place, change kDebugLevel, kDebugSpawnRow, and kDebugSpawnCol.

    // kDebugReleaseFinalSeal:
    //   true  = floor 24 enemy id 81 becomes OCTOPUS, the final boss.
    //   false = floor 24 enemy id 81 stays as DRAGON.
    // Before submitting the final version, set kDebugMode to false.
    const bool kDebugMode = false;
    const int kDebugLevel = 3;
    const int kDebugSpawnRow = 1;
    const int kDebugSpawnCol = 5;
    const bool kDebugReleaseFinalSeal = false;

    // Debug player stats.
    // Set kDebugOverridePlayerStats to true when you want to test battle/shop floors quickly.
    const bool kDebugOverridePlayerStats = true;
    const int kDebugHp = 3000;
    const int kDebugAtk = 300;
    const int kDebugDef = 300;
    const int kDebugExp = 300;
    const int kDebugCoin = 300;
    const int kDebugYellowKeys = 10;
    const int kDebugBlueKeys = 10;
    const int kDebugRedKeys = 10;

    // Debug special items.
    const bool kDebugHasWindCompass = true;   // J: 樓層飛升
    const bool kDebugHasMonsterBook = true;   // L: 怪物手冊
    const bool kDebugOverrideMaxReachedFloor = true;
    const int kDebugMaxReachedFloor = 0;
    const bool kDebugCross = true; // 十字架
    const bool kDebugBlueVeri = true; // 冰之靈杖
    const bool kDebugRedVeri = true; // 炎之靈杖
    const bool kDebugGreenVeri = true; // 心之靈杖

    if (kDebugMode) {
        if (kDebugReleaseFinalSeal) {
            m_GameMap.SetFinalSealReleased(true);
        }
        m_GameMap.SetLevel(kDebugLevel);
        spawnRow = kDebugSpawnRow;
        spawnCol = kDebugSpawnCol;
    }

    const float playerX = startX + (spawnCol * tileSize) + (tileSize / 2.0f);
    const float playerY = startY - (spawnRow * tileSize) - (tileSize / 2.0f);

    m_Player = std::make_shared<Player>();
    m_Player->SetPosition({playerX, playerY});
    m_Player->SetScale({1.6f, 1.6f});

    m_BattleScene = std::make_shared<BattleScene>();
    m_NPCDialog = std::make_shared<NPCDialog>();
    m_RewardMessage = std::make_shared<RewardMessage>();
    m_ShopScene = std::make_shared<ShopScene>();
    m_FloorTeleport = std::make_shared<FloorTeleport>();
    m_MonsterBook = std::make_shared<MonsterBook>();
    m_NoticeScene = std::make_shared<NoticeScene>();
    m_GameFlowScene = std::make_shared<GameFlowScene>();

    // Debug mode skips the opening scenes, so you can test maps, NPCs, shops,
    // enemies, or endings without replaying login/loading/intro every time.
    if (kDebugMode) {
        m_GameFlowScene->StartPlaying();
    }
    if (kDebugMode && kDebugOverridePlayerStats) {
        m_Player->m_Hp = kDebugHp;
        m_Player->m_Atk = kDebugAtk;
        m_Player->m_Def = kDebugDef;
        m_Player->m_Exp = kDebugExp;
        m_Player->m_Coin = kDebugCoin;
        m_Player->m_YellowKeys = kDebugYellowKeys;
        m_Player->m_BlueKeys = kDebugBlueKeys;
        m_Player->m_RedKeys = kDebugRedKeys;
    }
    if (kDebugMode) {
        m_Player->m_HasWindCompass = kDebugHasWindCompass;
        m_Player->m_HasMonsterBook = kDebugHasMonsterBook;
        m_Player->m_Cross = kDebugCross;
        m_Player->m_BlueVeri = kDebugBlueVeri;
        m_Player->m_RedVeri = kDebugRedVeri;
        m_Player->m_GreenVeri = kDebugGreenVeri;
    }

    m_Player->m_MaxReachedFloor = m_GameMap.GetCurrentLevel();

    if (kDebugMode && kDebugOverrideMaxReachedFloor) {
        m_Player->m_MaxReachedFloor = kDebugMaxReachedFloor;
    }

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    Util::Renderer renderer;

    const float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
        renderer.Update();
        return;
    }

    if (m_GameFlowScene) {
        m_GameFlowScene->Update(deltaTime);
        if (m_GameFlowScene->ConsumeRestartRequested()) {
            RestartGame();
            renderer.Update();
            return;
        }

        if (m_GameFlowScene->IsFinished()) {
            m_CurrentState = State::END;
            renderer.Update();
            return;
        }

        if (!m_GameFlowScene->ShouldDrawMap()) {
            m_GameFlowScene->Draw();
            renderer.Update();
            return;
        }
    }

    m_GameMap.UpdateAnimation(deltaTime);

    auto drawBase = [&]() {
        renderer.AddChild(m_MainBackground);
        m_GameMap.Draw();
        renderer.AddChild(m_Player);

        if (!m_GameFlowScene || m_GameFlowScene->ShouldShowStatusUI()) {
            m_UI.Draw();
        }
    };

    if (m_GameFlowScene && !m_GameFlowScene->CanUpdateGame()) {
        m_UI.Update(*m_Player, m_GameMap);
        drawBase();
        m_GameFlowScene->Draw();
        renderer.Update();
        return;
    }

    if (m_BattleScene->IsActive()) {
        m_BattleScene->Update(deltaTime);
        drawBase();
        m_BattleScene->Draw();

        renderer.Update();
        return;
    }

    if (m_BattleScene->ConsumeGameOverRequested()) {
        m_GameFlowScene->StartGameOver();
        drawBase();
        m_GameFlowScene->Draw();

        renderer.Update();
        return;
    }

    if (m_BattleScene->ConsumeFinalBossDefeated()) {
        m_GameFlowScene->StartWin();
        m_UI.Update(*m_Player, m_GameMap);
        drawBase();
        m_GameFlowScene->Draw();

        renderer.Update();
        return;
    }

    if (m_RewardMessage->IsVisible()) {
        m_RewardMessage->Update(deltaTime);
        drawBase();
        m_RewardMessage->Draw();

        renderer.Update();
        return;
    }

    if (m_NPCDialog->IsActive()) {
        m_NPCDialog->Update(deltaTime);
        drawBase();
        m_NPCDialog->Draw();

        renderer.Update();
        return;
    }

    if (m_ShopScene->IsActive()) {
        m_ShopScene->Update(deltaTime);
        m_UI.Update(*m_Player, m_GameMap);
        drawBase();
        m_ShopScene->Draw();

        renderer.Update();
        return;
    }

    if (m_FloorTeleport->IsActive()) {
        m_FloorTeleport->Update(deltaTime, m_GameMap, *m_Player);
        m_UI.Update(*m_Player, m_GameMap);
        drawBase();
        m_FloorTeleport->Draw();

        renderer.Update();
        return;
    }

    if (m_MonsterBook->IsActive()) {
        m_MonsterBook->Update(deltaTime);
        drawBase();
        m_MonsterBook->Draw();

        renderer.Update();
        return;
    }

    if (m_NoticeScene->IsActive()) {
        m_NoticeScene->Update(deltaTime);
        drawBase();
        m_NoticeScene->Draw();

        renderer.Update();
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::R)) {
        RestartGame();
        renderer.Update();
        return;
    }

    if (m_Player->m_HasWindCompass && Util::Input::IsKeyDown(Util::Keycode::J)) {
        m_FloorTeleport->Open(m_GameMap, *m_Player);
        drawBase();
        m_FloorTeleport->Draw();

        renderer.Update();
        return;
    }

    if (m_Player->m_HasMonsterBook && Util::Input::IsKeyDown(Util::Keycode::L)) {
        m_MonsterBook->Open(m_GameMap, *m_Player);
        drawBase();
        m_MonsterBook->Draw();

        renderer.Update();
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::N)) {
        m_NoticeScene->Open();
        drawBase();
        m_NoticeScene->Draw();

        renderer.Update();
        return;
    }

    m_Player->Update(m_GameMap, *m_BattleScene, *m_RewardMessage, *m_NPCDialog, *m_ShopScene);
    m_Player->m_MaxReachedFloor = std::max(m_Player->m_MaxReachedFloor, m_GameMap.GetCurrentLevel());
    m_UI.Update(*m_Player, m_GameMap);

    drawBase();
    renderer.Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

void App::RestartGame() {
    m_GameMap.Reset();
    Start();
}
