/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SCARLETM_H
#define DEF_SCARLETM_H

enum
{
    MAX_ENCOUNTER                   = 1,

    TYPE_MOGRAINE_AND_WHITE_EVENT   = 1,

    NPC_MOGRAINE                    = 3976,
    NPC_WHITEMANE                   = 3977,
    NPC_VORREL                      = 3981,
    NPC_INTERROGATOR_VISHAS         = 3983,
	NPC_MOGRAINE_TRANSFORM          = 16440,

    GO_WHITEMANE_DOOR               = 104600,

    SAY_TRIGGER_VORREL              = -1189015,
	AURA_ASHBRINGER                 = 28282,
	SPELL_ASHBRINGER                = 28441,
	FACTION_FRIENDLY                = 35,
	FACTION_HOSTILE                 = 67,
};
//Coords used to spawn Mograine at the entrance to Cathedral (stairs)																						
static const float aMograineSpawnPosition[4] = { 1065.13f, 1399.35f, 30.7637f,0.025f };
//Coords for end walk location for Mograine (inside Catehdral, stairs)
static const float aMograineThronePosition[4] = { 1150.40f, 1398.41f, 32.25f, 0.025f };
static bool m_hasashbringer;
class instance_scarlet_monastery : public ScriptedInstance
{
    public:
        instance_scarlet_monastery(Map* pMap);

        void Initialize() override;

		void OnPlayerEnter(Player* pPlayer) override;
        void OnCreatureCreate(Creature* pCreature) override;
        void OnCreatureDeath(Creature* pCreature) override;
        void OnObjectCreate(GameObject* pGo) override;

        void SetData(uint32 uiType, uint32 uiData) override;
        uint32 GetData(uint32 uiData) const override;

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
};

#endif
