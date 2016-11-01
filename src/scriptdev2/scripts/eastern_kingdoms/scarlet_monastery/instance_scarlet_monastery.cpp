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
}
