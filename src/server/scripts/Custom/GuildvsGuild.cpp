#include "ScriptPCH.h"
#include "../../game/GuildvsGuild/GuildvsGuildMgr.h"
#include <cstring>

#define GOSSIP_ITEM_1       "Arathibecken"
#define GOSSIP_ITEM_2       "Kriegshymnenschlucht"
#define GOSSIP_ITEM_3       "Arathibecken (Gruppe)"
#define GOSSIP_ITEM_4       "Kriegshymnenschlucht (Gruppe)"

class battlemaster_guildvsguild : public CreatureScript
{
    public:

        battlemaster_guildvsguild()
            : CreatureScript("battlemaster_guildvsguild")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
			if (player->isInCombat())
			{
				player->CLOSE_GOSSIP_MENU();
				creature->MonsterWhisper("Combat!", player->GetGUID());
				return true;
			}

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_2, GOSSIP_SENDER_MAIN, 2);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_3, GOSSIP_SENDER_MAIN, 3);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_4, GOSSIP_SENDER_MAIN, 4);

            player->PlayerTalkClass->SendGossipMenu(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
        {
			BattlegroundTypeId choice;
			bool isGrp;
            player->PlayerTalkClass->ClearMenus();
			if (player->isInCombat())
			{
				player->CLOSE_GOSSIP_MENU();
				creature->MonsterWhisper("Combat!", player->GetGUID());
				return true;
			}
			switch (uiAction)
			{
				case 1:
					choice = BATTLEGROUND_AB;
					isGrp = false;
					break;
				case 2:
					choice = BATTLEGROUND_WS;
					isGrp = false;
					break;
				case 3:
					choice = BATTLEGROUND_AB;
					isGrp = true;
					break;
				case 4:
					choice = BATTLEGROUND_WS;
					isGrp = true;
					break;
				default:
					choice = BATTLEGROUND_TYPE_NONE;
					break;
			}
			sGuildvsGuildMgr->BattlegroundGuildvsGuild(choice, player, isGrp);
			player->CLOSE_GOSSIP_MENU();
			return true;
        }
};

void AddSC_battlemaster_guildvsguild()
{
    new battlemaster_guildvsguild();
}