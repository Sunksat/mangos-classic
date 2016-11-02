/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
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

/* ScriptData
SDName: Instance_Scarlet_Monastery
SD%Complete: 50
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "precompiled.h"
#include "scarlet_monastery.h"

enum {
	//Mograine say on player enter with Ashbringer
	SAY_MOGRAINE_ASHBRINGER = -1999926,
	//Ashbringer NPC Says
	SAY_ASHBRINGER_1 = -1999927,
	SAY_ASHBRINGER_2 = -1999928,
	SAY_ASHBRINGER_3 = -1999929,
	SAY_ASHBRINGER_4 = -1999930,
	SAY_ASHBRINGER_5 = -1999931,
	SAY_ASHBRINGER_6 = -1999932,
	SAY_ASHBRINGER_7 = -1999933,
	// Scarlet Myrmidon & Spells
	NPC_SCARLET_MYRMIDON = 4295,
	// Scarlet Defender & Spells
	NPC_SCARLET_DEFENDER = 4298,
	SPELL_DEF_STANCE = 7164,
	SPELL_SH_BASH = 11972,
	SPELL_IMP_BLOCK = 3639,
	// Scarlet Sorcerer & Spells
	NPC_SCARLET_SORCERER = 4294,
	SPELL_FROSTBOLT = 9672,
	SPELL_SLOW = 6146,
	// Scarlet Wizard & Spells
	NPC_SCARLET_WIZARD = 4300,
	SPELL_FIRESHIELD_III = 2601,
	SPELL_ARCANE_EXPLOSION = 8439,
	// Scarlet Centurion & Spells
	SPELL_BATTLESHOUT = 9128,
	NPC_SCARLET_CENTURION = 4301,
	// Scarlet Abbot & Spells
	NPC_SCARLET_ABBOT = 4303,
	SPELL_HEAL = 6064,
	// Scarlet Monk & Spells
	NPC_SCARLET_MONK = 4540,
	SPELL_TRASH = 8876,
	SPELL_KICK = 11978,
	// Scarlet Chaplain & Spells
	NPC_SCARLET_CHAPLAIN = 4299,
	SPELL_PowerWordShield = 6066,
	// Scarlet Champion & Spells
	NPC_SCARLET_CHAMPION = 4302,
	SPELL_HOLYSTRIKE = 17143,
	// Common Spells
	SPELL_RENEW = 8362,
	SPELL_INNERFIRE = 1006,
	SPELL_FRENZY = 8269, // Not Working?
	FAIRBANKS_TRANSFORM_HUMAN = 28443, //ID: 16439
	SPELL_CURSEOFBLOOD = 8282,
	SPELL_DISPELMAGIC = 15090,
	SPELL_FEAR = 12096,
	SPELL_SLEEP = 8399,
	SPELL_FB_HEAL = 12039,
	SPELL_FB_POWERWORDSHIELD = 11647,
	// Common for most NPCs
	EMOTE_ENRAGE = -1000003,
	EMOTE_FLEE = -1000007,
	NPC_FLEE = 25,
	// Normal NPC Say on Aggro - Myrmidon, Defender, Centurion, Abbot, Monk, Chaplain, Champion
	RAN_SAY_AGGRO_1 = -1999940,
	RAN_SAY_AGGRO_2 = -1999941,
	RAN_SAY_AGGRO_3 = -1999942,
	RAN_SAY_AGGRO_4 = -1999943,
};

instance_scarlet_monastery::instance_scarlet_monastery(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_scarlet_monastery::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_scarlet_monastery::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_MOGRAINE:
        case NPC_WHITEMANE:
        case NPC_VORREL:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}
void instance_scarlet_monastery::OnPlayerEnter(Player* player)
{
	if (player->HasAura(AURA_ASHBRINGER) && m_auiEncounter[TYPE_MOGRAINE_AND_WHITE_EVENT] != DONE) // Check for Ashbringer Aura & SM is not Cleared
	{
		Creature* pMograine = GetSingleCreatureFromStorage(NPC_MOGRAINE);
		if (pMograine->isAlive())
		{
			DoOrSimulateScriptTextForThisInstance(SAY_MOGRAINE_ASHBRINGER, NPC_MOGRAINE);
		}
		m_hasashbringer = true;
	}
	else
	{
		m_hasashbringer = false;
	}
}
struct ashbringer_event : public ScriptedAI
{
	ashbringer_event(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
		switch (m_creature->GetEntry())
		{
		case NPC_SCARLET_ABBOT:
			DoCastSpellIfCan(m_creature, SPELL_INNERFIRE);
			break;
		case NPC_SCARLET_MONK:
			DoCastSpellIfCan(m_creature, SPELL_TRASH);
			break;
		case NPC_SCARLET_CHAPLAIN:
			DoCastSpellIfCan(m_creature, SPELL_INNERFIRE);
			break;
		}
	}
	bool m_AshSpellTriggerOnce;
	uint32 m_NpcKneel;
	uint32 m_NpcSay;
	uint32 m_uiSpeechTimer;
	uint8 m_uiSpeechNum;
	// Myrmidon
	float m_fHealthCheck;
	float m_fHealthFlee;
	// Defender
	uint32 m_uiDefensiveStance_Timer;
	uint32 m_uiShieldBash_Timer;
	uint32 m_ImprovedBlocking_Timer;
	// Sorcerer
	uint32 m_Frostbolt_Timer;
	uint32 m_Slow_Timer;
	float  m_fManaCheck;
	// Wizard
	uint32 m_FireShield_Timer;
	uint32 m_ArcaneExplosion_Timer;
	// Centurion
	uint32 m_BattleShout_Timer;
	// Abbot
	float m_fHealthHaflCheck;
	uint32 m_Heal_Timer;
	uint32 m_Renew_Timer;
	// Monk
	uint32 m_Kick_Timer;
	// Chaplain
	uint32 m_RenewChap_Timer;
	uint32 m_PowerWordShield_Timer;
	uint32 m_PowerWordShieldFriendly_Timer;
	// Champion
	uint32 m_HolyStrike_Timer;

	void Reset() override
	{

		m_uiSpeechTimer = 0;
		m_uiSpeechNum = 0;

		m_fHealthCheck = 30.0f;
		m_fHealthFlee = 15.0f;

		m_uiDefensiveStance_Timer = urand(2300, 6600);
		m_uiShieldBash_Timer = urand(4200, 14800);
		m_ImprovedBlocking_Timer = urand(8800, 13300);

		m_Frostbolt_Timer = urand(0, 40);
		m_Slow_Timer = urand(18100, 36200);

		m_FireShield_Timer = urand(1000, 5000);
		m_ArcaneExplosion_Timer = urand(0, 8);

		m_BattleShout_Timer = urand(4800, 13400);

		m_fHealthHaflCheck = 50.0f;
		m_Heal_Timer = 0;
		m_Renew_Timer = 0;

		m_Kick_Timer = urand(3600, 12800);

		m_RenewChap_Timer = 0;
		m_PowerWordShield_Timer = urand(1000, 6300);
		m_PowerWordShieldFriendly_Timer = urand(9600, 16900);

		m_HolyStrike_Timer = urand(8700, 14200);

		m_AshSpellTriggerOnce = false;

	}
	ScriptedInstance* m_pInstance;
	void Aggro(Unit* /*pWho*/) override
	{
		switch (m_creature->GetEntry())
		{
		case NPC_SCARLET_MYRMIDON:
			switch (urand(0, 25))
			{
			case 0: DoScriptText(RAN_SAY_AGGRO_1, m_creature); break;
			case 1: DoScriptText(RAN_SAY_AGGRO_2, m_creature); break;
			case 2: DoScriptText(RAN_SAY_AGGRO_3, m_creature); break;
			case 3: DoScriptText(RAN_SAY_AGGRO_4, m_creature); break;
			}
			break;
		case NPC_SCARLET_DEFENDER:
			switch (urand(0, 25))
			{
			case 0: DoScriptText(RAN_SAY_AGGRO_1, m_creature); break;
			case 1: DoScriptText(RAN_SAY_AGGRO_2, m_creature); break;
			case 2: DoScriptText(RAN_SAY_AGGRO_3, m_creature); break;
			case 3: DoScriptText(RAN_SAY_AGGRO_4, m_creature); break;
			}
			break;
		case NPC_SCARLET_CENTURION:
			switch (urand(0, 25))
			{
			case 0: DoScriptText(RAN_SAY_AGGRO_1, m_creature); break;
			case 1: DoScriptText(RAN_SAY_AGGRO_2, m_creature); break;
			case 2: DoScriptText(RAN_SAY_AGGRO_3, m_creature); break;
			case 3: DoScriptText(RAN_SAY_AGGRO_4, m_creature); break;
			}
			break;
		case NPC_SCARLET_ABBOT:
			switch (urand(0, 25))
			{
			case 0: DoScriptText(RAN_SAY_AGGRO_1, m_creature); break;
			case 1: DoScriptText(RAN_SAY_AGGRO_2, m_creature); break;
			case 2: DoScriptText(RAN_SAY_AGGRO_3, m_creature); break;
			case 3: DoScriptText(RAN_SAY_AGGRO_4, m_creature); break;
			}
			break;
		case NPC_SCARLET_MONK:
			switch (urand(0, 25))
			{
			case 0: DoScriptText(RAN_SAY_AGGRO_1, m_creature); break;
			case 1: DoScriptText(RAN_SAY_AGGRO_2, m_creature); break;
			case 2: DoScriptText(RAN_SAY_AGGRO_3, m_creature); break;
			case 3: DoScriptText(RAN_SAY_AGGRO_4, m_creature); break;
			}
			break;
		case NPC_SCARLET_CHAPLAIN:
			switch (urand(0, 25))
			{
			case 0: DoScriptText(RAN_SAY_AGGRO_1, m_creature); break;
			case 1: DoScriptText(RAN_SAY_AGGRO_2, m_creature); break;
			case 2: DoScriptText(RAN_SAY_AGGRO_3, m_creature); break;
			case 3: DoScriptText(RAN_SAY_AGGRO_4, m_creature); break;
			}
			break;
		case NPC_SCARLET_CHAMPION:
			switch (urand(0, 25))
			{
			case 0: DoScriptText(RAN_SAY_AGGRO_1, m_creature); break;
			case 1: DoScriptText(RAN_SAY_AGGRO_2, m_creature); break;
			case 2: DoScriptText(RAN_SAY_AGGRO_3, m_creature); break;
			case 3: DoScriptText(RAN_SAY_AGGRO_4, m_creature); break;
			}
			break;
		}

	}
	void SpellHit(Unit* pWho, const SpellEntry* pSpell) override
	{
		if (pWho->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_ASHBRINGER && !m_AshSpellTriggerOnce)
		{
			if (m_creature->getStandState() != UNIT_STAND_STATE_STAND) // Get NPCs in Cathedral to first stand before turning
			{
				m_creature->SetStandState(UNIT_STAND_STATE_STAND);
			}
			m_creature->GetMotionMaster()->MoveIdle();
			m_creature->StopMoving(true);
			m_creature->SetFacingToObject(pWho);
			m_AshSpellTriggerOnce = true;
			m_uiSpeechTimer = urand(2000, 3000); // Based on video, earliest kneel after being hit by Ashbringer Spell is 2secs(~) max is MAX 3secs(~)
			m_uiSpeechNum = 0;
		}
	}

	void UpdateAI(const uint32 uiDiff) override
	{
		if (m_hasashbringer)
		{
			m_creature->setFaction(FACTION_FRIENDLY);
			if (m_uiSpeechTimer && m_AshSpellTriggerOnce) // Do all the Ashbringer related texts here for normal NPCs
			{
				if (m_uiSpeechTimer <= uiDiff)
				{
					switch (m_uiSpeechNum)
					{
					case 0:
						m_creature->SetStandState(UNIT_STAND_STATE_KNEEL); // Pause between kneeling and saying
						m_uiSpeechTimer = urand(200, 600); // Almost instantly after kneeling the npcs do their say but at other times its with a slight dealy, hence 200-600.														   
						++m_uiSpeechNum;
						break;
					case 1:
						// Guesstimate via 2 videos, 47.54% chance of say (https://www.youtube.com/watch?v=24iX8QD3pRA) & (https://www.youtube.com/watch?v=fAW-EgWu3BU) //Vid1: Total Npc: 34, Total say: 18| 52%. Vid2: Total Npc: 27, say: 11| 40.7%
						switch (urand(0, 12))
						{
						case 0: DoScriptText(SAY_ASHBRINGER_1, m_creature); break;
						case 1: DoScriptText(SAY_ASHBRINGER_2, m_creature); break;
						case 2: DoScriptText(SAY_ASHBRINGER_3, m_creature); break;
						case 3: DoScriptText(SAY_ASHBRINGER_4, m_creature); break;
						case 4: DoScriptText(SAY_ASHBRINGER_5, m_creature); break;
						case 5: DoScriptText(SAY_ASHBRINGER_6, m_creature); break;
						case 6: DoScriptText(SAY_ASHBRINGER_7, m_creature); break;
						}
						m_uiSpeechTimer = 3000;
						++m_uiSpeechNum;
						break;
					case 2:
						// Jump out of loop
						m_uiSpeechTimer = 0;
						break;
					}
				}
				else
					m_uiSpeechTimer -= uiDiff;
			}
		}
		else // Do all the SM things that normally would occur IF player doesnt have Ashbringer Equipped
		{
			switch (m_creature->GetEntry())
			{
			case NPC_SCARLET_MYRMIDON:
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
				if (m_creature->GetHealthPercent() <= m_fHealthCheck)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_FRENZY) == CAST_OK)
						DoScriptText(EMOTE_ENRAGE, m_creature);
					m_fHealthCheck -= 30.0f;
				}
				if (m_creature->GetHealthPercent() <= m_fHealthFlee)
				{
					m_creature->DoFleeToGetAssistance();
					DoScriptText(EMOTE_FLEE, m_creature);
					m_fHealthFlee -= 15.0f;
				}
				else
					DoMeleeAttackIfReady();
				break;
			case NPC_SCARLET_DEFENDER:
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
				if (m_uiDefensiveStance_Timer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_DEF_STANCE) == CAST_OK)
						m_uiDefensiveStance_Timer = urand(180000, 186500);
				}
				else
					m_uiDefensiveStance_Timer -= uiDiff;
				if (m_uiShieldBash_Timer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SH_BASH) == CAST_OK)
						m_uiShieldBash_Timer = urand(9600, 18200);
				}
				else
					m_uiShieldBash_Timer -= uiDiff;
				if (m_ImprovedBlocking_Timer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_IMP_BLOCK) == CAST_OK)
						m_ImprovedBlocking_Timer = urand(13300, 19200);
				}
				else
					m_ImprovedBlocking_Timer -= uiDiff;
				if (m_creature->GetHealthPercent() <= m_fHealthFlee)
				{
					m_creature->DoFleeToGetAssistance();
					DoScriptText(EMOTE_FLEE, m_creature);
					m_fHealthFlee -= 15.0f;
				}
				else
					DoMeleeAttackIfReady();
				break;
			case NPC_SCARLET_SORCERER:
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
				if (m_Frostbolt_Timer < uiDiff)
				{
					if (m_creature->GetPower(POWER_MANA) * 100 / m_creature->GetMaxPower(POWER_MANA) >= 15.0f)
					{
						if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROSTBOLT) == CAST_OK)
							m_Frostbolt_Timer = urand(3600, 4800);
					}
					else
					{
						if (m_creature->GetPower(POWER_MANA) * 100 / m_creature->GetMaxPower(POWER_MANA) < 30.0f)
							DoMeleeAttackIfReady();
					}
				}
				else
					m_Frostbolt_Timer -= uiDiff;
				if (m_Slow_Timer < uiDiff)
				{
					if (m_creature->GetPower(POWER_MANA) * 100 / m_creature->GetMaxPower(POWER_MANA) >= 30.0f)
					{
						if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
						{
							if (DoCastSpellIfCan(pTarget, SPELL_SLOW) == CAST_INTERRUPT_PREVIOUS)
								m_Slow_Timer = urand(16900, 49600);
						}
					}
				}
				else
					m_Slow_Timer -= uiDiff;
				if (m_creature->GetHealthPercent() <= m_fHealthFlee)
				{
					m_creature->DoFleeToGetAssistance();
					DoScriptText(EMOTE_FLEE, m_creature);
					m_fHealthFlee -= 15.0f;
				}
				break;
			case NPC_SCARLET_WIZARD:
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
				if (m_FireShield_Timer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_FIRESHIELD_III) == CAST_OK)
						m_FireShield_Timer = urand(16900, 27500);
				}
				else
					m_FireShield_Timer -= uiDiff;
				if (m_ArcaneExplosion_Timer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION) == CAST_OK)
						m_ArcaneExplosion_Timer = urand(3600, 12100);
				}
				else
					m_ArcaneExplosion_Timer -= uiDiff;
				if (m_creature->GetHealthPercent() <= m_fHealthFlee)
				{
					m_creature->DoFleeToGetAssistance();
					DoScriptText(EMOTE_FLEE, m_creature);
					m_fHealthFlee -= 15.0f;
				}
				else
					DoMeleeAttackIfReady();
				break;
			case NPC_SCARLET_CENTURION:
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
				if (m_BattleShout_Timer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_BATTLESHOUT) == CAST_OK)
						m_BattleShout_Timer = urand(12100, 24600);
				}
				else
					m_BattleShout_Timer -= uiDiff;
				if (m_creature->GetHealthPercent() <= m_fHealthFlee)
				{
					m_creature->DoFleeToGetAssistance();
					DoScriptText(EMOTE_FLEE, m_creature);
					m_fHealthFlee -= 15.0f;
				}
				else
					DoMeleeAttackIfReady();
				break;
			case NPC_SCARLET_ABBOT:
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
				if (m_Heal_Timer < uiDiff)
				{
					if (Unit* pUnit = DoSelectLowestHpFriendly(40.0f, 1800))
					{
						if (DoCastSpellIfCan(pUnit, SPELL_HEAL) == CAST_OK)
							m_Heal_Timer = urand(13300, 19300);
					}
				}
				else
					m_Heal_Timer -= uiDiff;
				if (m_Renew_Timer < uiDiff)
				{
					if (m_creature->GetHealthPercent() <= m_fHealthHaflCheck)
					{
						if (DoCastSpellIfCan(m_creature, SPELL_RENEW) == CAST_INTERRUPT_PREVIOUS)
							m_Renew_Timer = urand(24100, 30200);
					}
				}
				else
					m_Renew_Timer -= uiDiff;
				if (m_creature->GetHealthPercent() <= m_fHealthCheck)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_FRENZY) == CAST_INTERRUPT_PREVIOUS)
						DoScriptText(EMOTE_ENRAGE, m_creature);
					m_fHealthCheck -= 30.0f;
				}
				if (m_creature->GetHealthPercent() <= m_fHealthFlee)
				{
					m_creature->DoFleeToGetAssistance();
					DoScriptText(EMOTE_FLEE, m_creature);
					m_fHealthFlee -= 15.0f;
				}
				else
					DoMeleeAttackIfReady();
				break;
			case NPC_SCARLET_MONK:
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
				if (m_Kick_Timer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_KICK) == CAST_OK)
						m_Kick_Timer = urand(3600, 12800);
				}
				else
					m_Kick_Timer -= uiDiff;
				if (m_creature->GetHealthPercent() <= m_fHealthFlee)
				{
					m_creature->DoFleeToGetAssistance();
					DoScriptText(EMOTE_FLEE, m_creature);
					m_fHealthFlee -= 15.0f;
				}
				else
					DoMeleeAttackIfReady();
				break;
			case NPC_SCARLET_CHAPLAIN:
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
				if (m_PowerWordShield_Timer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_PowerWordShield) == CAST_OK)
						m_PowerWordShield_Timer = urand(40000, 51000);
				}
				else
					m_PowerWordShield_Timer -= uiDiff;
				if (m_PowerWordShieldFriendly_Timer < uiDiff)
				{
					Creature* pTarget = nullptr;
					std::list<Creature*> pList = DoFindFriendlyMissingBuff(15.0f, SPELL_PowerWordShield);
					if (!pList.empty())
					{
						std::list<Creature*>::iterator i = pList.begin();
						advance(i, (rand() % pList.size()));
						pTarget = (*i);
					}
					if (!pTarget)
						pTarget = m_creature;

					if (DoCastSpellIfCan(pTarget, SPELL_PowerWordShield) == CAST_OK)
						m_PowerWordShieldFriendly_Timer = urand(9700, 16900);
				}
				else
					m_PowerWordShieldFriendly_Timer -= uiDiff;
				if (m_RenewChap_Timer < uiDiff)
				{
					if (Unit* pUnit = DoSelectLowestHpFriendly(20.0f, 1200))
					{
						if (DoCastSpellIfCan(pUnit, SPELL_RENEW) == CAST_INTERRUPT_PREVIOUS)
							m_RenewChap_Timer = urand(22900, 28800);
					}
				}
				else
					m_RenewChap_Timer -= uiDiff;
				if (m_creature->GetHealthPercent() <= m_fHealthFlee)
				{
					m_creature->DoFleeToGetAssistance();
					DoScriptText(EMOTE_FLEE, m_creature);
					m_fHealthFlee -= 15.0f;
				}
				else
					DoMeleeAttackIfReady();
				break;
			case NPC_SCARLET_CHAMPION:
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;
				if (m_HolyStrike_Timer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_HOLYSTRIKE) == CAST_OK)
						m_HolyStrike_Timer = urand(18100, 27300);
				}
				else
					m_HolyStrike_Timer -= uiDiff;
				if (m_creature->GetHealthPercent() <= m_fHealthFlee)
				{
					m_creature->DoFleeToGetAssistance();
					DoScriptText(EMOTE_FLEE, m_creature);
					m_fHealthFlee -= 15.0f;
				}
				else
					DoMeleeAttackIfReady();
				break;
			}
		}
	}
};
CreatureAI* GetAI_ashbringer_event(Creature* pCreature)
{
	return new ashbringer_event(pCreature);
}
// npc_high_inquisitor_fairbanks - Gossip Select Option for Ashbringer
#define GOSSIP_ITEM_ASH1 "Curse? What's going on here, Fairbanks?"
#define GOSSIP_ITEM_ASH2 "Mograine?"
#define GOSSIP_ITEM_ASH3 "What do you mean?"
#define GOSSIP_ITEM_ASH4 "I still do not fully understand."
#define GOSSIP_ITEM_ASH5 "Incredible story. So how did he die?"
#define GOSSIP_ITEM_ASH6 "You mean..."
#define GOSSIP_ITEM_ASH7 "How do you know all of this?"
#define GOSSIP_ITEM_ASH8 "A thousand? For one man?"
#define GOSSIP_ITEM_ASH9 "Yet? Yet What??"
#define GOSSIP_ITEM_ASH10 "And did he?"
#define GOSSIP_ITEM_ASH11 "Continue please, Fairbanks."
#define GOSSIP_ITEM_ASH12 "You mean..."
#define GOSSIP_ITEM_ASH13 "You were right, Fairbanks. That is tragic."
#define GOSSIP_ITEM_ASH14 "And you did..."
#define GOSSIP_ITEM_ASH15 "You tell an incredible tale, Fairbanks. What of the blade? Is it beyond redemption?"
#define GOSSIP_ITEM_ASH16 "But his son is dead."
struct high_inquisitor_fairbanksAI : public ScriptedAI
{
	high_inquisitor_fairbanksAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}
	bool m_AshSpellTriggerOnce;
	uint32 m_transformspell;
	ScriptedInstance* m_pInstance;
	float m_fHealthCheckHeal;
	float m_fHealthCheckShield;

	uint32 m_CurseofBlood_Timer;
	uint32 m_DispelMagic_Timer;
	uint32 m_CastFear_Timer;
	uint32 m_CastSleep_Timer;

	void Reset() override
	{
		m_CurseofBlood_Timer = urand(2800, 11900);
		m_DispelMagic_Timer = urand(3200, 11300);
		m_CastFear_Timer = 40000;
		m_CastSleep_Timer = 30000;

		m_fHealthCheckHeal = 30.0f;
		m_fHealthCheckShield = 25.0f;

		m_AshSpellTriggerOnce = false;
	}
	void SpellHit(Unit* pWho, const SpellEntry* pSpell) override
	{
		if (pWho->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_ASHBRINGER && !m_AshSpellTriggerOnce && m_creature->IsWithinDistInMap(pWho, 7.0f)) // && pWho->IsWithinLOS(1167.1f, 1354.52f, 31.54f  Spell ignores Line of Sight hence we need to ensure player is in the room with Fairbanks
		{
			m_creature->SetFacingToObject(pWho);
			m_transformspell = urand(3000, 4000); // Guesttimate based on: (https://www.youtube.com/watch?v=24iX8QD3pRA) end of video, from turning to player till transform 2-4secs
			m_AshSpellTriggerOnce = true;
		}
	}

	void UpdateAI(const uint32 uiDiff) override
	{
		if (m_hasashbringer) // Do all the SM things that normally would occur IF player has Ashbringer
		{
			m_creature->setFaction(FACTION_FRIENDLY);
			if (m_AshSpellTriggerOnce)
			{
				if (m_transformspell)
				{
					if (m_transformspell <= uiDiff)
					{
						m_creature->CastSpell(m_creature, FAIRBANKS_TRANSFORM_HUMAN, true);
						m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
						m_transformspell = 0;
					}
					else
						m_transformspell -= uiDiff;
				}
			}

		}
		else // Do all the SM things that normally would occur IF player doesnt have Ashbringer
		{
			if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;
			if (m_CurseofBlood_Timer < uiDiff)
			{
				if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CURSEOFBLOOD) == CAST_OK)
					m_CurseofBlood_Timer = urand(6100, 20500);
			}
			else
				m_CurseofBlood_Timer -= uiDiff;
			if (m_DispelMagic_Timer < uiDiff)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				{
					if (DoCastSpellIfCan(pTarget, SPELL_DISPELMAGIC) == CAST_OK)
						m_DispelMagic_Timer = urand(5700, 19300);
				}
			}
			else
				m_DispelMagic_Timer -= uiDiff;
			if (m_CastFear_Timer < uiDiff)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
				{
					if (DoCastSpellIfCan(pTarget, SPELL_FEAR) == CAST_OK)
						m_CastFear_Timer = 40000;
				}
			}
			else
				m_CastFear_Timer -= uiDiff;
			if (m_CastSleep_Timer < uiDiff)
			{
				if (DoCastSpellIfCan(m_creature, SPELL_SLEEP) == CAST_OK)
					m_CastSleep_Timer = 40000;
			}
			else
				m_CastSleep_Timer -= uiDiff;
			if (m_creature->GetHealthPercent() <= m_fHealthCheckHeal)
			{
				if (DoCastSpellIfCan(m_creature, SPELL_FB_HEAL) == CAST_INTERRUPT_PREVIOUS)
					m_fHealthCheckHeal -= 30.0f;
			}
			if (m_creature->GetHealthPercent() <= m_fHealthCheckShield)
			{
				if (DoCastSpellIfCan(m_creature, SPELL_FB_POWERWORDSHIELD) == CAST_OK)
					m_fHealthCheckShield -= 25.0f;
			}
			else
				DoMeleeAttackIfReady();
		}
	}
};
CreatureAI* GetAI_high_inquisitor_fairbanks(Creature* pCreature)
{
	return new high_inquisitor_fairbanksAI(pCreature);
}
bool GossipHello_high_inquisitor_fairbanksAI(Player* pPlayer, Creature* pCreature)
{
	if (m_hasashbringer && pPlayer->HasAura(AURA_ASHBRINGER))
	{
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(70000, pCreature->GetObjectGuid());

	}
	else
	{
		pPlayer->SEND_GOSSIP_MENU(70000, pCreature->GetObjectGuid()); // If another player is present, Fairbanks has the first say and nothing else
	}
	return true;

}
bool GossipSelect_high_inquisitor_fairbanksAI(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
{
	if (m_hasashbringer && pPlayer->HasAura(AURA_ASHBRINGER))
	{
		switch (uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF + 1:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			pPlayer->SEND_GOSSIP_MENU(70001, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 2:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
			pPlayer->SEND_GOSSIP_MENU(70002, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 3:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
			pPlayer->SEND_GOSSIP_MENU(70003, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 4:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
			pPlayer->SEND_GOSSIP_MENU(70004, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 5:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
			pPlayer->SEND_GOSSIP_MENU(70005, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 6:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
			pPlayer->SEND_GOSSIP_MENU(70006, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 7:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH8, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
			pPlayer->SEND_GOSSIP_MENU(70007, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 8:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH9, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
			pPlayer->SEND_GOSSIP_MENU(70008, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 9:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH10, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
			pPlayer->SEND_GOSSIP_MENU(70009, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 10:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH11, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
			pPlayer->SEND_GOSSIP_MENU(70010, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 11:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH12, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
			pPlayer->SEND_GOSSIP_MENU(70011, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 12:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH13, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
			pPlayer->SEND_GOSSIP_MENU(70012, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 13:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH14, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
			pPlayer->SEND_GOSSIP_MENU(70013, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 14:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH15, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
			pPlayer->SEND_GOSSIP_MENU(70014, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 15:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ASH16, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
			pPlayer->SEND_GOSSIP_MENU(70015, pCreature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF + 16:
			pPlayer->SEND_GOSSIP_MENU(70016, pCreature->GetObjectGuid());
			break;
		}
		return true;
	}

}
void instance_scarlet_monastery::OnCreatureDeath(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_INTERROGATOR_VISHAS)
    {
        // Any other actions to do with Vorrel? setStandState?
        if (Creature* pVorrel = GetSingleCreatureFromStorage(NPC_VORREL))
            DoScriptText(SAY_TRIGGER_VORREL, pVorrel);
    }
}

void instance_scarlet_monastery::OnObjectCreate(GameObject* pGo)
{
    if (pGo->GetEntry() == GO_WHITEMANE_DOOR)
        m_mGoEntryGuidStore[GO_WHITEMANE_DOOR] = pGo->GetObjectGuid();
}

void instance_scarlet_monastery::SetData(uint32 uiType, uint32 uiData)
{
    if (uiType == TYPE_MOGRAINE_AND_WHITE_EVENT)
    {
        if (uiData == IN_PROGRESS)
            DoUseDoorOrButton(GO_WHITEMANE_DOOR);
        if (uiData == FAIL)
            DoUseDoorOrButton(GO_WHITEMANE_DOOR);

        m_auiEncounter[0] = uiData;
    }
}

uint32 instance_scarlet_monastery::GetData(uint32 uiData) const
{
    if (uiData == TYPE_MOGRAINE_AND_WHITE_EVENT)
        return m_auiEncounter[0];

    return 0;
}

InstanceData* GetInstanceData_instance_scarlet_monastery(Map* pMap)
{
    return new instance_scarlet_monastery(pMap);
}

void AddSC_instance_scarlet_monastery()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_scarlet_monastery";
    pNewScript->GetInstanceData = &GetInstanceData_instance_scarlet_monastery;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "ashbringer_event";
	pNewScript->GetAI = &GetAI_ashbringer_event;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "high_inquisitor_fairbanksAI";
	pNewScript->GetAI = &GetAI_high_inquisitor_fairbanks;
	pNewScript->pGossipHello = &GossipHello_high_inquisitor_fairbanksAI;
	pNewScript->pGossipSelect = &GossipSelect_high_inquisitor_fairbanksAI;
	pNewScript->RegisterSelf();
}
