/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifndef SC_DEFINES_H
#define SC_DEFINES_H

#include "../ScriptMgr.h"

// Gossip Text
#define GOSSIP_TEXT_BANK                "The Bank"
#define GOSSIP_TEXT_WINDRIDER           "Wind rider master"
#define GOSSIP_TEXT_GRYPHON             "Gryphon Master"
#define GOSSIP_TEXT_BATHANDLER          "Bat Handler"
#define GOSSIP_TEXT_HIPPOGRYPH          "Hippogryph Master"
#define GOSSIP_TEXT_AUCTIONHOUSE        "Auction House"
#define GOSSIP_TEXT_GUILDMASTER         "Guild Master"
#define GOSSIP_TEXT_INN                 "The Inn"
#define GOSSIP_TEXT_MAILBOX             "Mailbox"
#define GOSSIP_TEXT_STABLEMASTER        "Stable Master"
#define GOSSIP_TEXT_WEAPONMASTER        "Weapons Trainer"
#define GOSSIP_TEXT_BATTLEMASTER        "Battlemaster"
#define GOSSIP_TEXT_CLASSTRAINER        "Class Trainer"
#define GOSSIP_TEXT_PROFTRAINER         "Profession Trainer"
#define GOSSIP_TEXT_OFFICERS            "The officers` lounge"

#define GOSSIP_TEXT_ALTERACVALLEY       "Alterac Valley"
#define GOSSIP_TEXT_ARATHIBASIN         "Arathi Basin" 
#define GOSSIP_TEXT_WARSONGULCH         "Warsong Gulch"

#define GOSSIP_TEXT_DRUID               "Druid" 
#define GOSSIP_TEXT_HUNTER              "Hunter"
#define GOSSIP_TEXT_PRIEST              "Priest"
#define GOSSIP_TEXT_ROGUE               "Rogue"
#define GOSSIP_TEXT_WARRIOR             "Warrior"
#define GOSSIP_TEXT_PALADIN             "Paladin"
#define GOSSIP_TEXT_SHAMAN              "Shaman"
#define GOSSIP_TEXT_MAGE                "Mage"
#define GOSSIP_TEXT_WARLOCK             "Warlock"


#define GOSSIP_TEXT_ALCHEMY             "Alchemy"
#define GOSSIP_TEXT_BLACKSMITHING       "Blacksmithing"
#define GOSSIP_TEXT_COOKING             "Cooking"
#define GOSSIP_TEXT_ENCHANTING          "Enchanting"
#define GOSSIP_TEXT_ENGINEERING         "Engineering"
#define GOSSIP_TEXT_FIRSTAID            "First Aid"
#define GOSSIP_TEXT_HERBALISM           "Herbalism"
#define GOSSIP_TEXT_LEATHERWORKING      "Leatherworking"
#define GOSSIP_TEXT_POISONS             "Poisons"
#define GOSSIP_TEXT_TAILORING           "Tailoring"
#define GOSSIP_TEXT_MINING              "Mining"
#define GOSSIP_TEXT_FISHING             "Fishing"
#define GOSSIP_TEXT_SKINNING            "Skinning"

#define GOSSIP_TEXT_IRONFORGE_BANK      "Bank of Ironforge"
#define GOSSIP_TEXT_STORMWIND_BANK      "Bank of Stormwind"
#define GOSSIP_TEXT_DEEPRUNTRAM         "Deeprun Tram"
#define GOSSIP_TEXT_ZEPPLINMASTER       "Zeppelin master"
#define GOSSIP_TEXT_FERRY               "Rut'theran Ferry"

// Skill defines

#define TRADESKILL_ALCHEMY                  1
#define TRADESKILL_BLACKSMITHING            2
#define TRADESKILL_COOKING                  3
#define TRADESKILL_ENCHANTING               4
#define TRADESKILL_ENGINEERING              5
#define TRADESKILL_FIRSTAID                 6
#define TRADESKILL_HERBALISM                7
#define TRADESKILL_LEATHERWORKING           8
#define TRADESKILL_POISONS                  9
#define TRADESKILL_TAILORING                10
#define TRADESKILL_MINING                   11
#define TRADESKILL_FISHING                  12
#define TRADESKILL_SKINNING                 13

#define TRADESKILL_LEVEL_NONE               0
#define TRADESKILL_LEVEL_APPRENTICE         1
#define TRADESKILL_LEVEL_JOURNEYMAN         2
#define TRADESKILL_LEVEL_EXPERT             3
#define TRADESKILL_LEVEL_ARTISAN            4

// Dialog defines

#define DIALOG_STATUS_NONE                  0
#define DIALOG_STATUS_UNAVAILABLE           1
#define DIALOG_STATUS_CHAT                  2
#define DIALOG_STATUS_INCOMPLETE            3
#define DIALOG_STATUS_REWARD_REP            4
#define DIALOG_STATUS_AVAILABLE             5
#define DIALOG_STATUS_REWARD                6

// Gossip defines

#define GOSSIP_ACTION_TRADE                 1
#define GOSSIP_ACTION_TRAIN                 2
#define GOSSIP_ACTION_TAXI                  3
#define GOSSIP_ACTION_GUILD                 4
#define GOSSIP_ACTION_BATTLE                5
#define GOSSIP_ACTION_BANK                  6
#define GOSSIP_ACTION_INN                   7
#define GOSSIP_ACTION_HEAL                  8
#define GOSSIP_ACTION_TABARD                9
#define GOSSIP_ACTION_AUCTION               10
#define GOSSIP_ACTION_INN_INFO              11
#define GOSSIP_ACTION_UNLEARN               12
#define GOSSIP_ACTION_INFO_DEF              1000

#define GOSSIP_SENDER_MAIN                  1
#define GOSSIP_SENDER_INN_INFO              2
#define GOSSIP_SENDER_INFO                  3
#define GOSSIP_SENDER_SEC_PROFTRAIN         4
#define GOSSIP_SENDER_SEC_CLASSTRAIN        5
#define GOSSIP_SENDER_SEC_BATTLEINFO        6

#define DEFAULT_GOSSIP_MESSAGE              0xffffff

extern uint32 GetSkillLevel(Player *player,uint32 skill);

// Defined fuctions to use with player.

// This fuction add's a menu item,
// a - Icon Id
// b - Text
// c - Sender(this is to identify the current Menu with this item)
// d - Action (identifys this Menu Item)
#define ADD_GOSSIP_ITEM(a,b,c,d)   PlayerTalkClass->GetGossipMenu()->AddMenuItem(a,b,c,d)

// This fuction Sends the current menu to show to client, a - NPCTEXTID(uint32) , b - npc guid(uint64)
#define SEND_GOSSIP_MENU(a,b)      PlayerTalkClass->SendGossipMenu(a,b)

// This fuction shows POI(point of interest) to client.
// a - position X
// b - position Y
// c - Icon Id
// d - Flags
// e - Data
// f - Location Name
#define SEND_POI(a,b,c,d,e,f)      PlayerTalkClass->SendPointOfInterest(a,b,c,d,e,f)

// Closes the Menu
#define CLOSE_GOSSIP_MENU()        PlayerTalkClass->CloseGossip()

// Fuction to tell to client the details
// a - quest object
// b - npc guid(uint64)
// c - Activate accept(bool)
#define SEND_QUEST_DETAILS(a,b,c)  PlayerTalkClass->SendQuestDetails(a,b,c)

// Fuction to tell to client the requested items to complete quest
// a - quest object
// b - npc guid(uint64)
// c - Iscompletable(bool)
// d - close at cancel(bool) - in case single incomplite ques
#define SEND_REQUESTEDITEMS(a,b,c,d) PlayerTalkClass->SendRequestedItems(a,b,c,d)

// Fuctions to send NPC lists, a - is always the npc guid(uint64)
#define SEND_VENDORLIST(a)         GetSession()->SendListInventory(a)
#define SEND_TRAINERLIST(a)        GetSession()->SendTrainerList(a)
#define SEND_BANKERLIST(a)         GetSession()->SendShowBank(a)
#define SEND_TABARDLIST(a)         GetSession()->SendTabardVendorActivate(a)
#define SEND_AUCTIONLIST(a)        GetSession()->SendAuctionHello(a)
#define SEND_TAXILIST(a)           GetSession()->SendTaxiStatus(a)

// Ressurect's the player if is dead.
#define SEND_SPRESURRECT()         GetSession()->SendSpiritResurrect()

// Get the player's honor rank.
#define GET_HONORRANK()            GetHonorRank()
// -----------------------------------

// defined fuctions to use with Creature

#define QUEST_DIALOG_STATUS(a,b)   getDialogStatus(a,b)
#endif
