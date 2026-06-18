#include "Map/Map.hpp"
#include "System/BattleScene.hpp"
#include "System/RewardMessage.hpp"
#include "NPC/NPCDialog.hpp"
#include "Character/Player.hpp"

#include "Util/Time.hpp"
#include "Util/TransformUtils.hpp"
#include <algorithm>

BattleScene::BattleScene() {
    m_BackgroundImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Character/Enemy/Fighting.bmp");

    m_Drawable = m_BackgroundImage;

    m_VsText = std::make_shared<TextObject>(55, "VS", 51.0f);
    m_VsText->m_Transform.translation = {0.0f, 20.0f};


    m_Transform.translation = {0.8f, 0.6f};
    m_ZIndex = 50.0f;

    int fontSize = 32;
    float z = 51.0f;

    m_EnemyHpLabel = std::make_shared<TextObject>(fontSize, "體力：", z);
    m_EnemyAtkLabel = std::make_shared<TextObject>(fontSize, "攻擊力：", z);
    m_EnemyDefLabel = std::make_shared<TextObject>(fontSize, "防禦力：", z);

    m_PlayerHpLabel = std::make_shared<TextObject>(fontSize, "：體力", z);
    m_PlayerAtkLabel = std::make_shared<TextObject>(fontSize, "：攻擊力", z);
    m_PlayerDefLabel = std::make_shared<TextObject>(fontSize, "：防禦力", z);

    m_EnemyNameText = std::make_shared<TextObject>(fontSize, "Enemy", z);
    m_EnemyHpText = std::make_shared<TextObject>(fontSize, "0", z);
    m_EnemyAtkText = std::make_shared<TextObject>(fontSize, "0", z);
    m_EnemyDefText = std::make_shared<TextObject>(fontSize, "0", z);

    m_PlayerNameText = std::make_shared<TextObject>(fontSize, "勇者", z);
    m_PlayerHpText = std::make_shared<TextObject>(fontSize, "0", z);
    m_PlayerAtkText = std::make_shared<TextObject>(fontSize, "0", z);
    m_PlayerDefText = std::make_shared<TextObject>(fontSize, "0", z);

    float farX = 210.0f;
    float nearX = 100.0f;
    float startY = 70.0f;
    float spacing = 55.0f;


    m_EnemyHpLabel->m_Transform.translation = {-farX, startY};
    m_EnemyHpText->m_Transform.translation  = {-nearX, startY};

    m_EnemyAtkLabel->m_Transform.translation = {-farX, startY - spacing};
    m_EnemyAtkText->m_Transform.translation  = {-nearX, startY - spacing};

    m_EnemyDefLabel->m_Transform.translation = {-farX, startY - spacing * 2};
    m_EnemyDefText->m_Transform.translation  = {-nearX, startY - spacing * 2};

    m_PlayerHpText->m_Transform.translation  = {nearX, startY};
    m_PlayerHpLabel->m_Transform.translation = {farX, startY};

    m_PlayerAtkText->m_Transform.translation  = {nearX, startY - spacing};
    m_PlayerAtkLabel->m_Transform.translation = {farX, startY - spacing};

    m_PlayerDefText->m_Transform.translation  = {nearX, startY - spacing * 2};
    m_PlayerDefLabel->m_Transform.translation = {farX, startY - spacing * 2};

    m_EnemyNameText->m_Transform.translation = {-340.0f, 120.0f};
    m_PlayerNameText->m_Transform.translation = {355.0f, 120.0f};


    m_ZIndex = 50.0f;
    m_Active = false;
    this->SetVisible(false);

    m_RewardMessage = std::make_shared<RewardMessage>();
}

void BattleScene::StartBattle(Player& player, Map& map, std::shared_ptr<Enemy> enemy, NPCDialog& npcDialog) {
    m_Player = &player;
    m_Map = &map;
    m_NPCDialog = &npcDialog;
    m_Enemy = enemy;
    m_EnemyStats = enemy->GetStats();
    m_EnemyType = enemy->GetType();
    m_DefeatedEnemyGrid = map.GetGridPosition(
        enemy->m_Transform.translation.x,
        enemy->m_Transform.translation.y
    );
    m_FinalBossDefeated = false;
    m_GameOverRequested = false;

    if (m_EnemyType == Enemy::Type::DRAGON) {
        m_GameOverRequested = true;
        m_Active = false;
        m_State = State::IDLE;
        this->SetVisible(false);
        return;
    }

    m_EnemyAvatarFrame1 = std::make_shared<Util::Image>(m_EnemyStats.imagePath1);
    m_EnemyAvatarFrame2 = std::make_shared<Util::Image>(m_EnemyStats.imagePath2.empty() ? m_EnemyStats.imagePath1 : m_EnemyStats.imagePath2);
    m_EnemyAvatar = m_EnemyAvatarFrame1;
    m_PlayerAvatar = std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Character/Player/player_d1.png");

    m_EnemyNameText->SetText(m_EnemyStats.name);
    m_EnemyAtkText->SetText(std::to_string(m_EnemyStats.atk));
    m_EnemyDefText->SetText(std::to_string(m_EnemyStats.def));

    m_PlayerAtkText->SetText(std::to_string(m_Player->m_Atk));
    m_PlayerDefText->SetText(std::to_string(m_Player->m_Def));

    m_CurrentPlayerHp = player.m_Hp - m_Enemy->GetOpeningDamage(player.m_Hp);
    m_CurrentEnemyHp = m_EnemyStats.hp;
    m_PlayerHpText->SetText(std::to_string(m_CurrentPlayerHp));
    m_EnemyHpText->SetText(std::to_string(m_CurrentEnemyHp));
    
    m_Active = true;
    m_State = State::FIGHTING;
    m_Timer = 0.0f;
    m_EnemyAnimationTimer = 0.0f;
    m_ShowEnemyAltFrame = false;
    m_PostBattleEventStarted = false;

    this->SetVisible(true);
}

void BattleScene::Update(float deltaTime) {
    if (!m_Active) return;

    if (m_State == State::FIGHTING) {
        m_EnemyAnimationTimer += deltaTime;
        if (m_EnemyAnimationTimer >= m_EnemyAnimationInterval) {
            m_EnemyAnimationTimer = 0.0f;
            m_ShowEnemyAltFrame = !m_ShowEnemyAltFrame;
            m_EnemyAvatar = m_ShowEnemyAltFrame ? m_EnemyAvatarFrame2 : m_EnemyAvatarFrame1;
        }

        m_Timer += deltaTime;
        if (m_Timer >= m_AttackInterval) {
            m_Timer = 0.0f;


            int damageToEnemy = std::max(0, m_Player->m_Atk - m_EnemyStats.def);
            m_CurrentEnemyHp -= damageToEnemy;

            if (m_CurrentEnemyHp <= 0) {
                m_State = State::FINISHED;
                this->SetVisible(false);


                m_Player->m_Hp = m_CurrentPlayerHp;
                m_Player->m_Coin += m_EnemyStats.coin;
                m_Player->m_Exp += m_EnemyStats.exp;
                if (m_EnemyType == Enemy::Type::OCTOPUS) {
                    m_FinalBossDefeated = true;
                }

                if (m_Map && m_Enemy) {
                    m_Map->RemoveEnemy(m_Enemy);
                    m_Enemy.reset();
                }

                std::string rewardMsg = "擊殺成功! 獲得 金幣: " + std::to_string(m_EnemyStats.coin) +
                                           "  經驗值: " + std::to_string(m_EnemyStats.exp);
                m_RewardMessage->Show(rewardMsg);
                return;
            }


            int damageToPlayer = std::max(0, m_EnemyStats.atk - m_Player->m_Def);
            m_CurrentPlayerHp -= damageToPlayer;

            m_PlayerHpText->SetText(std::to_string(m_CurrentPlayerHp));
            m_EnemyHpText->SetText(std::to_string(m_CurrentEnemyHp));
        }
    }
    else if (m_State == State::FINISHED) {

        m_RewardMessage->Update(deltaTime);

        if (!m_RewardMessage->IsVisible()) {
            if (!m_PostBattleEventStarted &&
                m_Map &&
                m_NPCDialog &&
                m_Map->GetCurrentLevel() == 21 &&
                m_DefeatedEnemyGrid.x == 1 &&
                m_DefeatedEnemyGrid.y == 5) {
                m_PostBattleEventStarted = true;
                m_NPCDialog->Start({
                    {m_EnemyStats.name, "啊......怎麼可能，我怎麼可能會被你打敗呢！", m_EnemyStats.imagePath1},
                    {m_EnemyStats.name, "不...不要這樣......", m_EnemyStats.imagePath1}
                }, [map = m_Map]() {
                    if (map) {
                        map->SetTileAtLevel(21, 0, 5, 5);
                        map->SetTileAtLevel(21, 6, 5, 0);
                    }
                });
            }

            m_Active = false;
            m_State = State::IDLE;
        }
    }
}

void BattleScene::Draw() {
    if (!m_Active) return;

    if (m_State == State::FIGHTING && m_Visible) {
        auto bgMatrices = Util::ConvertToUniformBufferData(
        m_Transform,
        m_BackgroundImage->GetSize(),
        m_ZIndex
        );
        m_BackgroundImage->Draw(bgMatrices);
        m_VsText->Draw();

        m_EnemyHpLabel->Draw();
        m_EnemyAtkLabel->Draw();
        m_EnemyDefLabel->Draw();
        m_PlayerHpLabel->Draw();
        m_PlayerAtkLabel->Draw();
        m_PlayerDefLabel->Draw();


        m_EnemyHpText->Draw();
        m_EnemyAtkText->Draw();
        m_EnemyDefText->Draw();
        m_PlayerHpText->Draw();
        m_PlayerAtkText->Draw();
        m_PlayerDefText->Draw();
        m_EnemyNameText->Draw();
        m_PlayerNameText->Draw();


        if (m_EnemyAvatar) {
            Util::Transform enemyT;
            enemyT.translation = {-345.0f, 38.0f};
            enemyT.scale = {2.0f, 2.0f};
            m_EnemyAvatar->Draw(Util::ConvertToUniformBufferData(enemyT, m_EnemyAvatar->GetSize(), m_ZIndex + 1));
        }


        if (m_PlayerAvatar) {
            Util::Transform playerT;
            playerT.translation = {345.0f, 38.0f};
            playerT.scale = {2.0f, 2.0f};
            m_PlayerAvatar->Draw(Util::ConvertToUniformBufferData(playerT, m_PlayerAvatar->GetSize(), m_ZIndex + 1));
        }
    }

    if (m_State == State::FINISHED) {
        m_RewardMessage->Draw();
    }
}
