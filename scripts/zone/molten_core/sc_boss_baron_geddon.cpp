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

#define SPELL_INFERNO               19695
#define SPELL_IGNITEMANA            19659
#define SPELL_LIVINGBOMB            20475
#define SPELL_ARMAGEDDOM            20479

struct MANGOS_DLL_DECL boss_baron_geddonAI : public ScriptedAI
{
    boss_baron_geddonAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Inferno_Timer;
    uint32 IgniteMana_Timer;
    uint32 LivingBomb_Timer;
    bool InCombat;

    void Reset()
    {
        Inferno_Timer = 45000;      //These times are probably wrong
        IgniteMana_Timer = 30000;
        LivingBomb_Timer = 35000;
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

            //If we are <2% hp cast Armageddom
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 2 && !m_creature->m_currentSpell)
            {
                DoCast(m_creature,SPELL_ARMAGEDDOM);
                DoTextEmote("performs one last service for Ragnaros.",NULL);
                return;
            }
            
            //Inferno_Timer
            if (Inferno_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_INFERNO);

                //7 seconds until we should cast this agian
                Inferno_Timer = 45000;
            }else Inferno_Timer -= diff;

            //IgniteMana_Timer
            if (IgniteMana_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_IGNITEMANA);

                //35 seconds until we should cast this agian
                IgniteMana_Timer = 30000;
            }else IgniteMana_Timer -= diff;

            //LivingBomb_Timer
            if (LivingBomb_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_LIVINGBOMB);

                //30 seconds until we should cast this agian
                LivingBomb_Timer = 35000;
            }else LivingBomb_Timer -= diff;

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
CreatureAI* GetAI_boss_baron_geddon(Creature *_Creature)
{
    return new boss_baron_geddonAI (_Creature);
}


void AddSC_boss_baron_geddon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_baron_geddon";
    newscript->GetAI = GetAI_boss_baron_geddon;
    m_scripts[nrscripts++] = newscript;
}