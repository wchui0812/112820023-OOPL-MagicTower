#include "System/CollisionHandler.hpp"
#include "System/Battle.hpp"
#include "System/BattleScene.hpp"
#include "NPC/NPCEventManager.hpp"

#include "Util/Logger.hpp"

#include <cmath>

bool CollisionHandler::HandleCollision(
    Player& player,
    Map& map,
    const glm::vec2& targetPos,
    BattleScene& battleScene,
    RewardMessage& rewardMessage,
    NPCDialog& npcDialog,
    ShopScene& shopScene
) {
    auto enemy = map.GetEnemyAt(targetPos.x, targetPos.y);
    if (enemy) {
        if (enemy->GetType() == Enemy::Type::DRAGON) {
            battleScene.StartBattle(player, map, enemy, npcDialog);
            return false;
        }

        if (Battle::CanWin(player, *enemy)) {
            // 播放動畫
            battleScene.StartBattle(player, map, enemy, npcDialog);
            return false;
        }
        return false;
    }

    auto npc = map.GetNPCAt(targetPos.x, targetPos.y);
    if (npc) {
        NPCEventManager::DispatchEvent(npc->GetType(), player, map, npc, npcDialog, shopScene, rewardMessage);
        return false;
    }

    int tileType = map.GetTileType(targetPos.x, targetPos.y);

    // 1. 處理鑰匙 (10-12)
    if (tileType >= 10 && tileType <= 12) {
        if (tileType == 10) {
            player.m_YellowKeys++;
            rewardMessage.ShowItem("獲得黃鑰匙! 黃鑰匙 +1");
        }
        else if (tileType == 11) {
            player.m_BlueKeys++;
            rewardMessage.ShowItem("獲得藍鑰匙! 藍鑰匙 +1");
        }
        else if (tileType == 12) {
            player.m_RedKeys++;
            rewardMessage.ShowItem("獲得紅鑰匙! 紅鑰匙 +1");
        }
        map.RemoveTile(targetPos.x, targetPos.y);
        return false;
    }

    // 2. 處理屬性物品 (31-34)
    if (tileType >= 30 && tileType <= 50) {
        if (tileType == 30) {
            player.m_Coin += 300;
            rewardMessage.ShowItem("獲得金幣! 金幣 +300");
        }
        else if (tileType == 31) {
            player.m_Hp += 200;
            rewardMessage.ShowItem("獲得紅色生命藥水! 體力 +200");
        }
        else if (tileType == 32) {
            player.m_Hp += 500;
            rewardMessage.ShowItem("獲得藍色生命藥水! 體力 +500");
        }
        else if (tileType == 33) {
            player.m_Atk += 3;
            rewardMessage.ShowItem("獲得紅寶石! 攻擊力 +3");
        }
        else if (tileType == 34) {
            player.m_Def += 3;
            rewardMessage.ShowItem("獲得藍寶石! 防禦力 +3");
        }
        else if (tileType == 35) {
            player.m_Atk += 10;
            rewardMessage.ShowItem("獲得鐵劍! 攻擊力 +10");
        }
        else if (tileType == 36) {
            player.m_YellowKeys ++;
            player.m_BlueKeys ++;
            player.m_RedKeys ++;
            rewardMessage.ShowItem("獲得鑰匙盒! 所有顏色鑰匙 +1");
        }
        else if (tileType == 37) {
            player.m_Def += 10;
            rewardMessage.ShowItem("獲得鐵盾! 防禦力 +10");
        }
        else if (tileType == 38) {
            player.m_Level ++;
            player.m_Hp += 1000;
            player.m_Atk += 10;
            player.m_Def += 10;
            rewardMessage.ShowItem("獲得小飛羽! 跳躍一級!");
        }
        else if (tileType == 39) {
            player.m_Cross = true;
            rewardMessage.ShowItem("獲得幸運十字架！將它交給序章中的仙子，可以將自身的所有能力提升三分之一");
        }
        else if (tileType == 40) {
            player.m_HasWindCompass = true;
            rewardMessage.ShowItem("獲得風之羅盤! 以J啟動，可以自由飛往去過的樓層");
        }
        else if (tileType == 41) {
            player.m_Atk += 70;
            rewardMessage.ShowItem("獲得青鋒劍! 攻擊力 +70");
        }
        else if (tileType == 42) {
            player.m_Def += 30;
            rewardMessage.ShowItem("獲得鋼盾! 防禦力 +10");
        }
        else if (tileType == 43) {
            player.m_HasGemDigger = true;
            rewardMessage.ShowItem("獲得星光神鋃! 把它交給第四層的小偷，小偷便會用它打開第十八層的隱藏地面");
        }
        else if (tileType == 44) {
            player.m_Level += 3;
            player.m_Hp += 3000;
            player.m_Atk += 30;
            player.m_Def += 30;
            rewardMessage.ShowItem("獲得大飛羽! 跳躍三級!");
        }
        else if (tileType == 45) {
            player.m_Hp *= 2;
            rewardMessage.ShowItem("獲得聖水! 體力變兩倍!");
        }
        else if (tileType == 46) {
            player.m_Atk += 150;
            rewardMessage.ShowItem("獲得星光神劍! 攻擊力 +150");
        }
        else if (tileType == 47) {
            player.m_Def += 190;
            rewardMessage.ShowItem("獲得神聖盾! 防禦力 +190");
        }
        else if (tileType == 48) {
            player.m_RedVeri = true;
            rewardMessage.ShowItem("獲得炎之靈杖!");
        }
        else if (tileType == 49) {
            player.m_GreenVeri = true;
            rewardMessage.ShowItem("獲得心之靈杖!");
        }
        else if (tileType == 50) {
            player.m_HasMonsterBook = true;
            rewardMessage.ShowItem("獲得聖光徽! 以L使用，使用後可以查看怪物基本情況及能力");
        }

        map.RemoveTile(targetPos.x, targetPos.y);
        LOG_INFO("Status Boost! HP: {}, ATK: {}, DEF: {}", player.m_Hp, player.m_Atk, player.m_Def);
        return false;
    }

    // 3. 處理門 (21-25)
    if (tileType >= 21 && tileType <= 25) {
        bool canOpen = false;
        const int currentLevel = map.GetCurrentLevel();
        const bool isLayer23Series = currentLevel == 23 || currentLevel == 25 || currentLevel == 26;
        const bool isFreeIronFenceLevel = currentLevel == 2 || currentLevel == 4 || currentLevel == 7 || currentLevel == 10 || currentLevel == 13 || currentLevel == 14 || currentLevel == 18;

        if (tileType == 21 && player.m_YellowKeys > 0) { player.m_YellowKeys--; canOpen = true; }
        else if (tileType == 22 && player.m_BlueKeys > 0) { player.m_BlueKeys--; canOpen = true; }
        else if (tileType == 23 && player.m_RedKeys > 0) { player.m_RedKeys--; canOpen = true; }
        else if (tileType == 24 && currentLevel == 23) { canOpen = true; }
        else if (tileType == 25 && isFreeIronFenceLevel) { canOpen = true; }
        else if (tileType == 25 && isLayer23Series && map.GetEnemies().empty()) { canOpen = true; }
        

        if (canOpen) {
            const int replacementTile = (tileType == 24 && currentLevel == 23) ? 5 : 0;
            map.TriggerDoorAnimation(targetPos.x, targetPos.y, tileType, replacementTile);
        }
        return false; // 動畫播放中玩家不能立即踩上去
    }

    // 4. 普通行走判定 (0: 地板, 4/5: 樓梯)
    const bool canWalk = map.IsWalkable(targetPos.x, targetPos.y);
    const glm::ivec2 targetGrid = map.GetGridPosition(targetPos.x, targetPos.y);
    if (canWalk &&
        map.GetCurrentLevel() == 16 &&
        targetGrid.x == 3 &&
        targetGrid.y == 5 &&
        !player.m_RedDemon16DialogShown) {
        player.m_RedDemon16DialogShown = true;
        npcDialog.Start({
            {"紅衣魔王", "停止吧，愚蠢的人類！", RESOURCE_DIR "/Image/Character/Enemy/MagicSergeantA1.png"},
            {"勇士", "該停止的是你！魔王。快說，公主關在哪裡？", RESOURCE_DIR "/Image/Character/Player/player_d1.png"},
            {"紅衣魔王", "等你打贏我再說吧！", RESOURCE_DIR "/Image/Character/Enemy/MagicSergeantA1.png"},
        });
    }

    return canWalk;
}
