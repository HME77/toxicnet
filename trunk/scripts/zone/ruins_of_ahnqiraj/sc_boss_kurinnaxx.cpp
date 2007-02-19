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

#include "../../sc_defines.h"

// **** This script is still under Developement ****
/*
http://www.wowguru.com/db/mobs/kurinnaxx-id15348/
SandTrap(Blows up) - http://www.wowguru.com/db/spells/sand-trap-id25656/
Mortal Wound - http://www.wowguru.com/db/spells/mortal-wound-id36814/
Wide Slash - http://www.wowguru.com/db/spells/wide-slash-id25814/

*/

#define SPELL_SANDTRAP        25656
#define SPELL_MORTALWOUND         36814
#define SPELL_WIDESLASH         25814

struct MANGOS_DLL_DECL boss_kurinnaxxAI : public ScriptedAI
{
    boss_kurinnaxxAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 SandTrap_Timer;
    uint32 MortalWound_Timer;
	uint32 WideSlash_Timer;
	bool InCombat;

    void Reset()
    {
        pTarget = NULL;
        SandTrap_Timer = 20000;      //These times are probably wrong
        MortalWound_Timer = 15000;
		WideSlash_Timer = 10000;
		InCombat = false;
        if (m_creature)
        {
            EnterEvadeMode();
        }
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

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

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
            
            //SandTrap_Timer
            if (SandTrap_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SANDTRAP);

                //20 seconds until we should cast this agian
                SandTrap_Timer = 20000;
            }else SandTrap_Timer -= diff;

            //MortalWound_Timer
            if (MortalWound_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_MORTALWOUND);

                //15 secs and recast
                MortalWound_Timer = 15000;
            }else MortalWound_Timer -= diff;
			
            //WideSlash_Timer
            if (WideSlash_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_WIDESLASH);

                //10 secs and recast
                WideSlash_Timer = 10000;
            }else WideSlash_Timer -= diff;
			
			
            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
}; 
CreatureAI* GetAI_boss_kurinnaxx(Creature *_Creature)
{
    return new boss_kurinnaxxAI (_Creature);
}


void AddSC_boss_kurinnaxx()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kurinnaxx";
    newscript->GetAI = GetAI_boss_kurinnaxx;
    m_scripts[nrscripts++] = newscript;
}