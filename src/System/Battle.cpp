#include "System/Battle.hpp"

#include <cmath>

bool Battle::CanWin(const Player& player, const Enemy& enemy) {
    const auto& eStats = enemy.GetStats();
    
    int damageToEnemy = player.m_Atk - eStats.def;
    int damageToPlayer = eStats.atk - player.m_Def;
    if (damageToPlayer < 0) damageToPlayer = 0;

    // 判斷 1：如果攻擊力低於敵人防禦，永遠打不死
    if (damageToEnemy <= 0) return false;

    // 判斷 2：計算回合與玩家損血
    int rounds = (eStats.hp + damageToEnemy - 1) / damageToEnemy;
    int totalDamage = enemy.GetOpeningDamage(player.m_Hp) + (rounds - 1) * damageToPlayer;

    // 判斷 3：血量是否足夠支撐到戰鬥結束
    return player.m_Hp > totalDamage;
}

void Battle::ExecuteBattle(Player& player, const Enemy& enemy) {
    const auto& eStats = enemy.GetStats();
    
    int damageToEnemy = player.m_Atk - eStats.def;
    int damageToPlayer = (eStats.atk - player.m_Def > 0) ? (eStats.atk - player.m_Def) : 0;

    int rounds = (eStats.hp + damageToEnemy - 1) / damageToEnemy;
    int totalDamage = enemy.GetOpeningDamage(player.m_Hp) + (rounds - 1) * damageToPlayer;

    // 更新玩家數值
    player.m_Hp -= totalDamage;
    player.m_Coin += eStats.coin;
    player.m_Exp += eStats.exp;
}
