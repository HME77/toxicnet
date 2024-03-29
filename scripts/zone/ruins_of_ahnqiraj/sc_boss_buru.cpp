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
// Asta nu se poate face blizzlike cu suportul pe care il in prezent.
/*
SPELL_ID_CREEPING_PLAGUE = 20512
THORNS http://thottbot.com/?sp=25777
SPELL_ID_DISMEMBER = 96
*/

#define SPELL_PLAGUE        20512
#define SPELL_DISMEMBER        96
#define SPELL_THORNS        25777


struct MANGOS_DLL_DECL boss_buruAI : public ScriptedAI
{
    boss_buruAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Plague_Timer;
	uint32 Dismember_Timer;
	uint32 Thorns_Timer;
	bool InCombat;
    

    void Reset()
    {
        Plague_Timer = 150000;      //These times are probably wrong
		Dismember_Timer = 10000;
		Thorns_Timer = 5000;
	    InCombat = false;
        

        if (m_creature)
           EnterEvadeMode();
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

        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
				InCombat = true;

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
             //Plague_Timer
            if (Plague_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_PLAGUE);
                Plague_Timer = 15000;
            }else Plague_Timer -= diff;
			
			//Dismember_Timer
            if (Dismember_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_DISMEMBER);               
                Dismember_Timer = 10000;
				
            }else Plague_Timer -= diff;
			
			//Thorns_Timer
            if (Thorns_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_THORNS);                
                Thorns_Timer = 5000;
            }else Thorns_Timer -= diff;
            

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
CreatureAI* GetAI_boss_buru(Creature *_Creature)
{
    return new boss_buruAI (_Creature);
}


void AddSC_boss_buru()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_buru";
    newscript->GetAI = GetAI_boss_buru;
    m_scripts[nrscripts++] = newscript;
}