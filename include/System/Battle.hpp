#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "Character/Player.hpp"
#include "Enemy/Enemy.hpp"

class Battle {
public:

    static bool CanWin(const Player& player, const Enemy& enemy);

    static void ExecuteBattle(Player& player, const Enemy& enemy);
};

#endif