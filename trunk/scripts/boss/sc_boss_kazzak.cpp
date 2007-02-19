
/** Copyright © 2005,2006 ScriptDev <https://opensvn.csie.org/ScriptDev/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "../sc_defines.h"

#define SPELL_SHADOWBOLTVOLLEY 21341
#define SPELL_TWISTEDREFLECTION 21064
#define SPELL_VOIDBOLT 21066
#define SPELL_CLEAVE 8255
#define SPELL_MARKOFKAZZAK 21056
#define SPELL_THUNDERCLAP 23931
#define SPELL_CAPTURESOUL 21054


struct MANGOS_DLL_DECL boss_kazzakAI : public ScriptedAI
{
	 boss_kazzakAI(Creature *c) : ScriptedAI(c) {Reset();}

Unit *pTarget;
uint32 SHADOWBOLTVOLLEY_Timer;
uint32 TWISTEDREFLECTION_Timer;
uint32 VOIDBOLT_Timer;
uint32 CLEAVE_Timer;
uint32 MARKOFKAZZAK_Timer;
uint32 THUNDERCLAP_Timer;
uint32 supreme_Timer;
uint32 supreme;
bool InCombat;

void Reset()
{
pTarget = NULL;
SHADOWBOLTVOLLEY_Timer = 20000;
TWISTEDREFLECTION_Timer = 30000;
VOIDBOLT_Timer = 25000;
CLEAVE_Timer = 35000;
MARKOFKAZZAK_Timer = 40000;
THUNDERCLAP_Timer = 45000;
supreme_Timer = 180000;
supreme=0;
InCombat = false;


    if (m_creature->isAlive())
     {   EnterEvadeMode();   }
}

void AttackStart(Unit *who)
{
if (!who)
return;

if (m_creature->getVictim() == NULL && who->isTargetableForAttack() && who!= m_creature)
{
//Begin melee attack if we are within range
DoStartMeleeAttack(who);
InCombat = true;
pTarget = who;
}
}

void MoveInLineOfSight(Unit *who) {
	if (!who || m_creature->getVictim())
	return;

	if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who)) {
		float attackRadius = m_creature->GetAttackDistance(who);
		if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE) {
			if(who->HasStealthAura())
			who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

//Begin melee attack if we are within range
			DoStartMeleeAttack(who);
			InCombat = true;
			pTarget = who;
			}
		}
	}

void UpdateAI(const uint32 diff)
{
		//If we had a target and it wasn't cleared then it means the target died from some unknown soruce
        //But we still need to reset
        if (InCombat && !m_creature->SelectHostilTarget())
        {
            Reset();
            return;
        }

//Check if we have a current target
if( m_creature->getVictim() && m_creature->isAlive())
{
//Check if we should stop attacking because our victim is no longer attackable
if (needToStop())
{
Reset();
return;
}

		if (SHADOWBOLTVOLLEY_Timer < diff)
            {
               DoCast(m_creature->getVictim(),SPELL_SHADOWBOLTVOLLEY);
               SHADOWBOLTVOLLEY_Timer = 20000;
            }else SHADOWBOLTVOLLEY_Timer -= diff;

		if (TWISTEDREFLECTION_Timer < diff)
            {
               DoCast(m_creature->getVictim(),SPELL_TWISTEDREFLECTION);
               TWISTEDREFLECTION_Timer = 30000;
            }else TWISTEDREFLECTION_Timer -= diff;

		if (VOIDBOLT_Timer < diff)
            {
               DoCast(m_creature->getVictim(),SPELL_VOIDBOLT);
               if (supreme==1)
				{
					VOIDBOLT_Timer = 3000;
				}else VOIDBOLT_Timer = 25000;
            }else VOIDBOLT_Timer -= diff;

		if (CLEAVE_Timer < diff)
            {
               DoCast(m_creature->getVictim(),SPELL_CLEAVE);
               CLEAVE_Timer = 35000;
            }else CLEAVE_Timer -= diff;

		if (MARKOFKAZZAK_Timer < diff)
            {
               DoCast(m_creature->getVictim(),SPELL_MARKOFKAZZAK);
               MARKOFKAZZAK_Timer = 40000;
            }else MARKOFKAZZAK_Timer -= diff;

		if (THUNDERCLAP_Timer < diff)
            {
               DoCast(m_creature->getVictim(),SPELL_THUNDERCLAP);
               THUNDERCLAP_Timer = 45000;
            }else THUNDERCLAP_Timer -= diff;

		if (supreme_Timer < diff)
            {
               supreme=1;
            }else supreme_Timer -= diff;

	if(!pTarget->isAlive())
	{
		DoCast(m_creature,SPELL_CAPTURESOUL);

		m_creature->AttackerStateUpdate(m_creature->getVictim());
		m_creature->resetAttackTimer();
	}



if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST)) {
	if( m_creature->isAttackReady() && !m_creature->m_currentSpell) {
		m_creature->AttackerStateUpdate(m_creature->getVictim());
		m_creature->resetAttackTimer();
	}
}


}
}
}; 

CreatureAI* GetAI_boss_kazzak(Creature *_Creature)
{
return new boss_kazzakAI (_Creature);
}


void AddSC_boss_kazzak()
{
Script *newscript;
newscript = new Script;
newscript->Name="boss_kazzak";
newscript->GetAI = GetAI_boss_kazzak;
m_scripts[nrscripts++] = newscript;
}