#ifndef BATTLE_SCENE_HPP
#define BATTLE_SCENE_HPP

#include "System/TextObject.hpp"
#include "System/RewardMessage.hpp"
#include "Enemy/Enemy.hpp"

#include "Util/GameObject.hpp"
#include <glm/vec2.hpp>
#include <memory>

class Player;
class Map;
class NPCDialog;

class BattleScene : public Util::GameObject {
public:
    enum class State { IDLE, FIGHTING, FINISHED };

    BattleScene();

    void StartBattle(Player& player, Map& map, std::shared_ptr<Enemy> enemy, NPCDialog& npcDialog);
    
    void Update(float deltaTime);
    void Draw();
    
    bool IsActive() const { return m_Active; }
    State GetState() const { return m_State; }
    bool ConsumeFinalBossDefeated() {
        const bool defeated = m_FinalBossDefeated;
        m_FinalBossDefeated = false;
        return defeated;
    }
    bool ConsumeGameOverRequested() {
        const bool requested = m_GameOverRequested;
        m_GameOverRequested = false;
        return requested;
    }

private:
    bool m_Active = false;
    State m_State = State::IDLE;
    float m_Timer = 0.0f;
    float m_EnemyAnimationTimer = 0.0f;
    const float m_EnemyAnimationInterval = 0.35f;
    bool m_ShowEnemyAltFrame = false;
    const float m_AttackInterval = 0.5f;


    Player* m_Player = nullptr;
    Map* m_Map = nullptr;
    NPCDialog* m_NPCDialog = nullptr;
    std::shared_ptr<Enemy> m_Enemy = nullptr;
    Enemy::Stats m_EnemyStats;
    Enemy::Type m_EnemyType = Enemy::Type::GREEN_SLIME;
    glm::ivec2 m_DefeatedEnemyGrid = {-1, -1};
    bool m_PostBattleEventStarted = false;
    bool m_FinalBossDefeated = false;
    bool m_GameOverRequested = false;


    int m_CurrentPlayerHp = 0;
    int m_CurrentEnemyHp = 0;

    std::shared_ptr<Util::Image> m_BackgroundImage;

    std::shared_ptr<TextObject> m_VsText;

    std::shared_ptr<Util::Image> m_PlayerAvatar;
    std::shared_ptr<Util::Image> m_EnemyAvatar;
    std::shared_ptr<Util::Image> m_EnemyAvatarFrame1;
    std::shared_ptr<Util::Image> m_EnemyAvatarFrame2;


    std::shared_ptr<TextObject> m_EnemyNameText;
    std::shared_ptr<TextObject> m_EnemyHpText;
    std::shared_ptr<TextObject> m_EnemyAtkText;
    std::shared_ptr<TextObject> m_EnemyDefText;

    std::shared_ptr<TextObject> m_PlayerNameText;
    std::shared_ptr<TextObject> m_PlayerHpText;
    std::shared_ptr<TextObject> m_PlayerAtkText;
    std::shared_ptr<TextObject> m_PlayerDefText;

    std::shared_ptr<TextObject> m_EnemyHpLabel;
    std::shared_ptr<TextObject> m_EnemyAtkLabel;
    std::shared_ptr<TextObject> m_EnemyDefLabel;

    std::shared_ptr<TextObject> m_PlayerHpLabel;
    std::shared_ptr<TextObject> m_PlayerAtkLabel;
    std::shared_ptr<TextObject> m_PlayerDefLabel;

    std::shared_ptr<RewardMessage> m_RewardMessage;
};

#endif
