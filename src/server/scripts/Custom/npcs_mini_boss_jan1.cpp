/*########################################################
#                     Custom Bosse                       #
#             Extended-WoW 			Trinity Core         #
#             codet by DopeAttack                        #
# 														 #
########################################################*/

/* ScriptData
SDName: Extended-WoW Custom Bosse
SDAuthor: DopeAttack
SDComment: 
SDCategory: Eigene Bosse
EndScriptData */

#include "ScriptPCH.h"

// Saphos Deklaration
enum SaphosSpells
{
	BERSERKER				= 						28131, // unter 5% life 
	FROSTATEM				=						28524, // alle 2,5 minuten
	SCHWANZFEGER			=						55697, // immer
	SPALTEN					=						53631, // alle 30 sec
	INTENSIVE_KAELTE		=						48094
};

// Saphos Aggro Schreie
#define SAY_SAPHOS_AGGRO			"Ihr werdet mich nie besiegen!"
#define SAY_SAPHOS_BERSERK			"Jetzt bin ich Wütend!"
#define SAY_SAPHOS_FREEZE			"Spürt die Macht der Frostzauber!"
#define SAY_SAPHOS_DEATH			"Arh...Saa.."

// Hakkar Deklaration
enum HakkarSpells 
{
	RASEREI						=					28131, // unter 5% life 
	TEUFELSFEUERBALL			=					66532, // 1,30 2 ziele
	FLUCH_VON_HAKKAR			=					18502, // alle 2,5 min 5 ziele
	TEUFELSHITZE				=					67044, // 30 sec
	TEUFELSFLAMMENSTOß			=					66495  // 1 min
};

// Hakkar Aggro Schreie
#define SAY_HAKKAR_AGGRO			"Ihr nichts nuzigen Kreaturen"
#define SAY_HAKKAR_SUMMON			"Diener zu mir"
#define SAY_HAKKAR_FIRE				"Brennt in den Flammen"
#define SAY_HAKKAR_BERSERK			"Spürt meinen Zorn"
#define SAY_HAKKAR_DEATH			"Wie nur?"

// Tau´dan Deklaration
enum TaudanSpells
{
	SCHATTENSCHLEIER		=						24647, // auf ziel mit aggro
	AURA_DER_DUNKELHEIT		=						69491, // aktiv bei kampfbegin (aura , DoCastAOE)!
	SÄULE_DES_LEIDS			=						59727,
	STRUM_DER_TRAUER		=						59772 // zufälliges ziel
};

- Säule des Leids (59727)			
- Schattenschleier (24674)			(auf ziel mit aggro)
- Sturm der Trauer (59772)			(auf zufälliges ziel)

// Tau'dan Schreie

#define SAY_TAUDAN_AGGRO		"Fühlt die Aura der Dunkelheit"
#define SAY_TAUDAN_DEATH		"Nein! ... wieso??"
#define SAY_TAUDAN_SCHLAG		"Spürt die Macht des Flammenschlags"

// Auraleus Deklaration

enum AuraleusSpells
{
	BLITZSCHLAGWELLE		=						24819, //alle 3 sec standartattacke
	STATISCHE_ÜBERLADUNG	=						59795,
	KETTENBLITZSCHLAG		=						63479,
	DONNER					=						53630,
	MANA_ENTZÜNDEN			=						46543, //selten alle 4 min zufälliges ziel
	ÜBERLADEN				=						63481, // bei 75% ,50%,25% 
	BLITZRANKEN				=						61887, // unter 5%
	RASEREI					=						57086 // unter 5%

};

#define SAY_AURALEUS_AGGRO
#define SAY_AURALEUS_DEATH
#define SAY_AURALEUS_BERSERK

// Kazragore Deklaration

enum KazagoreSpells
{
	ZERREIßENDES_SPALTEN	=						25174,
	TÖDLICHER_STOß			=						15708,
	DONNERKNALL				=						63757,
	WIRBELWIND				=						59322, // 75% 50% 25%
	RASEREI					=						28131 // unter 5%
};

#define SAY_KAZAGORE_AGGRO
#define SAY_KAZAGORE_BERSERK
#define	SAY_KAZAGORE_DEATH

// Nyrociel Deklaration

enum NyrocielSpells
{
	ZERREIßENDES_SPALTEN	=						25174,
	TÖDLICHER_STOß			=						15708,
	DONNERKNALL				=						63757,
	WIRBELWIND				=						59322, // 75% 50% 25%
	RASEREI					=						28131 // unter 5%
};

#define SAY_NYROCIEL_AGGRO
#define SAY_NYROCIEL_BERSERK
#define	SAY_NYROCIEL_DEATH

// Kirgam Deklaration
enum KirgamSpells
{
	GEISTERSTOß					=						
	GERINGE_WELLE_DER_HEILUNG	=
	ENTWAFFNEN					=
	
};



/*
Saphos Script
*/
class npc_saphos : public CreatureScript
{
public:
		npc_saphos() : CreatureScript("npc_saphos") { }
		
		CreatureAI* GetAI(Creature* pCreature)	const
		{
			return new npc_saphosAI	(pCreature);
		}
		
		struct npc_saphosAI : public ScriptedAI
		{
			npc_saphosAI(Creature *pCreature) : ScriptedAI(pCreature) {}
			
			uint32 FlammenTimer;
			uint32 SchwanzfegerTimer;
			uint32 SpaltenTimer;
			bool Berserk;	
			bool Kaelte;
			
			void Reset()
			{
				FlammenTimer		= 12000;
				SchwanzfegerTimer	= 5000;				
				SpaltenTimer		= 4500;
				Berserk				= false;
				Kaelte				= false;
			}
			
			void EnterCombat(Unit* /*who*/)
			{
				me->MonsterYell(SAY_SAPHOS_AGGRO, LANG_UNIVERSAL, 0);
				DoCastAOE(INTENSIVE_KAELTE);
				Kaelte = true;
			}
			
			void KilledUnit(Unit* victim)
			{
				me->MonsterYell(SAY_SAPHOS_FREEZE, LANG_UNIVERSAL, 0);
			}
			
			void JustDied(Unit* /*killer*/)
			{
				me->MonsterYell(SAY_SAPHOS_DEATH, LANG_UNIVERSAL, 0);
			}
			
			void UpdateAI(const uint32 uiDiff)
			{
				if(!UpdateVictim())
				return;
				
				// Spalten Timer
				if(SpaltenTimer <= uiDiff)
				{
					DoCast(SPALTEN);
					SpaltenTimer = 4500;
				}	else SpaltenTimer -= uiDiff;
				
				// Frostatem
				if(FlammenTimer <= uiDiff)
				{
					me->InterruptNonMeleeSpells(false);
					me->DeleteThreatList();
					if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
					DoCast(pTarget, FLAMMNENATEM);
					FlammenTimer = 12000;
				}	else FlammenTimer -= uiDiff;
				
				// Schwanzfeger
				if(SchwanzfegerTimer <= uiDiff)
				{
					DoCast(me, SCHWANZFEGER);
					SchwanzfegerTimer = 5000;
				}	else SchwanzfegerTimer -= uiDiff;
				
				//Berserk bei 5%
				if (HealthBelowPct(5))
				{
					me->MonsterYell(SAY_SAPHOS_BERSERK, LANG_UNIVERSAL, 0);
					DoCast(me, BERSERKER);
					Berserk = true;
				}
				
				DoMeleeAttackIfReady();			
			}
	};
};

// Hakkar Script
class npc_hakkar : public CreatureScript
{
public:
		npc_hakkar() : CreatureScript("npc_hakkar") { }
		
		CreatureAI* GetAI(Creature* pCreature)	const
		{
			return new npc_hakkarAI	(pCreature);
		}
		
		struct npc_hakkarAI : public ScriptedAI
		{
			npc_hakkarAI(Creature *pCreature) : ScriptedAI(pCreature) {}
			
			uint32 Phase1Timer;
			uint32 Phase2Timer;
			uint32 FluchvonHakkarTimer;
			uint32 TeufelshitzeTimer;
			uint32 TeufelsinfernoTimer;
			uint32 TeufelsjägerTimer;
			bool Berserk;
			bool Raserei;
			bool Phase1;
			
			void Reset()
			{	
				Phase1Timer				= 90000;
				Phase2Timer				= 60000;
				FluchvonHakkarTimer		= 17000;
				TeufelshitzeTimer		= 9000;
				TeufelsinfernoTimer		= 13000;
				TeufelsjägerTimer		= 25000;
				Berserk					= false;
				Raserei					= false;
				Phase1					= false;
			}
			
			void EnterCombat(Unit* /*who*/)
			{
				me->MonsterYell(SAY_HAKKAR_AGGRO, LANG_UNIVERSAL, 0);
				DoZoneInCombat();
			}
			
			void JustDied(Unit* /*killer*/)
			{
				me->MonsterYell(SAY_HAKKAR_DEATH, LANG_UNIVERSAL, 0);
			}
			
			void UpdateAI(const uint32 uiDiff)
			{
				if(!UpdateVictim())
				return;
				
				// Beschwörung Phase 2
				if(Phase2Timer <= uiDiff)
				{
					Phase1 = false;
					me->InterruptNonMeleeSpells(false);
					me->RemoveAllAuras();
					me->DeleteThreatList();
					// In Phase 2 4 Teufelsjäger
						 if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
							for (uint8 i = 1; i <= 4; i++)
							{
								me->SummonCreature(TEUFELSJÄGER_BESCHWÖREN, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
							}
					me->MonsterYell(SAY_HAKKAR_SUMMON, LANG_UNIVERSAL, 0);
					Phase2Timer = 60000;
				}	else Phase2Timer -= uiDiff;
				
				// Normale Phase
				
				if (Phase1Timer <= uiDiff)
				{				
					Phase1 = true;
					me->InterruptNonMeleeSpells(false);
					DoZoneInCombat();
					if (me->getThreatManager().isThreatListEmpty())
                    EnterEvadeMode();
					Phase1Timer = 90000;
				}	else Phase1Timer -=uiDiff;
				
				if (Phase1)
				{	
					// Teufelshitze
					if (TeufelshitzeTimer <= uiDiff)
				{
					me->InterruptNonMeleeSpells(false);
					DoCast(TEUFELSHITZE);
					TeufelshitzeTimer	= 9000;					
				}	else TeufelshitzeTimer -= uiDiff;
				
				// Teufelsinferno
				if (TeufelsinfernoTimer <= uiDiff)
				{
					me->InterruptNonMeleeSpells(false);
					DoCast(TEUFELSINFERNO);
					TeufelsinfernoTimer	= 13000;
				}	else TeufelsinfernoTimer -= uiDiff;
												
				// Fluch von Hakkar
				if(FluchvonHakkarTimer <= uiDiff)
				{
					if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
						DoCast(FLUCH_VON_HAKKAR);
						FluchvonHakkarTimer	= 17000;
				}	else FluchvonHakkarTimer -= uiDiff;
				
				if(HealthBelowPct(15))
				{
					me->DeleteThreatList();
					me->RemoveAllAuras();
					DoCast(me,RASEREI);
					Raserei = true;
				}
				
				// Berserker
				if(HealthBelowPct(5))
				{
					me->MonsterYell(SAY_HAKKAR_BERSERK, LANG_UNIVERSAL, 0);
					DoCast(me, BERSERK);
					Berserk = true;
				}
				
				DoMeleeAttackIfReady();
			}
		}
	};
};

// Tau'dan Script
class npc_taudan : public CreatureScript
{
public:
		npc_taudan() : CreatureScript("npc_taudan") { }
		
		CreatureAI* GetAI(Creature* pCreature)	const
		{
			return new npc_taudanAI	(pCreature);
		}
		
		struct npc_taudanAI : public ScriptedAI
		{
			npc_taudanAI(Creature *pCreature) : ScriptedAI(pCreature) {}
			
			uint32 SchattenschleierTimer;
			uint32 FlammenschlagTimer;
			bool AuraderDunkelheit;
			
		void Reset()
		{
			SchattenschleierTimer = 12000;
			FlammenschlagTimer	  = 8000;
			AuraderDunkelheit	  = false;
		}
		void EnterCombat(Unit* /*who*/)
			{
				me->MonsterYell(SAY_TAUDAN_AGGRO, LANG_UNIVERSAL, 0);
				DoZoneInCombat();
				DoCast(me, AURA_DER_DUNKELHEIT);
			}
			
		void JustDied(Unit* /*killer*/)
			{
				me->MonsterYell(SAY_TAUDAN_DEATH, LANG_UNIVERSAL, 0);
			}
			
		void UpdateAI(const uint32 uiDiff)
			{
				if(!UpdateVictim())
				return;
				
				if(FlammenschlagTimer <= uiDiff)
				{
				me->MonsterYell(SAY_TAUDAN_SCHLAG, LANG_UNIVERSAL, 0);
				DoCast(FLAMMENSCHLAG);
				FlammenschlagTimer = 8000;
				}	else FlammenschlagTimer -= uiDiff;
				
				if(SchattenschleierTimer <= uiDiff)
				{
					me->DeleteThreatList();
					me->RemoveAllAuras();
					if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
					DoCast(SCHATTENSCHLEIER);
					SchattenschleierTimer = 12000;
				}	else SchattenschleierTimer -= uiDiff;
				
				DoMeleeAttackIfReady();
			}
	};
};
		
	
void AddSC_npcs_mini_boss_jan()
{
    new npc_saphos;
	new npc_hakkar;
	new npc_taudan;
}