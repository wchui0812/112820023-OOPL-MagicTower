#include "System/CollisionHandler.hpp"
#include "Util/Logger.hpp"

#include <cmath>

bool CollisionHandler::HandleCollision(Player& player, Map& map, const glm::vec2& targetPos) {
    int tileType = map.GetTileType(targetPos.x, targetPos.y);

    // 1. 處理鑰匙 (10-12)
    if (tileType >= 10 && tileType <= 12) {
        if (tileType == 10) player.m_YellowKeys++;
        else if (tileType == 11) player.m_BlueKeys++;
        else if (tileType == 12) player.m_RedKeys++;
        map.RemoveTile(targetPos.x, targetPos.y);
        return true;
    }

    // 2. 處理屬性物品 (31-34)
    if (tileType >= 30 && tileType <= 49) {
        if (tileType == 30) player.m_Coin += 300;
        else if (tileType == 31) player.m_Hp += 200;
        else if (tileType == 32) player.m_Hp += 500;
        else if (tileType == 33) player.m_Atk += 3;
        else if (tileType == 34) player.m_Def += 3;
        else if (tileType == 35) player.m_Atk += 10;
        else if (tileType == 36) {
            player.m_YellowKeys ++;
            player.m_BlueKeys ++;
            player.m_RedKeys ++;
        }
        else if (tileType == 37) player.m_Def += 10;
        else if (tileType == 38) {
            player.m_Level ++;
            player.m_Hp += 1000;
            player.m_Atk += 10;
            player.m_Def += 10;
        }
        else if (tileType == 39) {
            player.m_Hp = std::ceil(player.m_Hp * 4 / 3);
            player.m_Atk = std::ceil(player.m_Atk * 4 / 3);
            player.m_Def = std::ceil(player.m_Def * 4 / 3);
        }
        else if (tileType == 41) player.m_Atk += 70;
        else if (tileType == 42) player.m_Def += 30;
        else if (tileType == 43) player.m_Atk += 7;
        else if (tileType == 44) {
            player.m_Level += 3;
            player.m_Hp += 3000;
            player.m_Atk += 30;
            player.m_Def += 30;
        }
        else if (tileType == 45) player.m_Hp *= 2;
        else if (tileType == 46) player.m_Atk += 150;
        else if (tileType == 47) player.m_Def += 190;

        map.RemoveTile(targetPos.x, targetPos.y);
        LOG_INFO("Status Boost! HP: {}, ATK: {}, DEF: {}", player.m_Hp, player.m_Atk, player.m_Def);
        return true;
    }

    // 3. 處理門 (21-25)
    if (tileType >= 21 && tileType <= 25) {
        bool canOpen = false;
        if (tileType == 21 && player.m_YellowKeys > 0) { player.m_YellowKeys--; canOpen = true; }
        else if (tileType == 22 && player.m_BlueKeys > 0) { player.m_BlueKeys--; canOpen = true; }
        else if (tileType == 23 && player.m_RedKeys > 0) { player.m_RedKeys--; canOpen = true; }
        // 補充：綠門(24)與鐵門(25)也可以在這裡加

        if (canOpen) {
            map.TriggerDoorAnimation(targetPos.x, targetPos.y, tileType);
        }
        return false; // 動畫播放中玩家不能立即踩上去
    }

    // 4. 普通行走判定 (0: 地板, 4/5: 樓梯)
    return map.IsWalkable(targetPos.x, targetPos.y);
}