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
SPELL_ID_STRENGTH_OF_OSSIRIAN = 25176
SPELL_ID_FIRE_WEAKNESS = 25177
SPELL_ID_FROST_WEAKNESS = 25178
SPELL_ID_NATURE_WEAKNESS = 25180
SPELL_ID_ARCANE_WEAKNESS = 25181
SPELL_ID_SHADOW_WEAKNESS = 25183
SPELL_ID_SUPER_CRYSTAL = 25186
SPELL_ID_WAR_STOMP_6 = 25188
SPELL_ID_ENVELOPING_WINDS = 25189
SPELL_ID_SUMMON_OSSIRIAN_CRYSTAL = 25192
SPELL_ID_DESPAWN_OSSIRIAN_CRYSTAL = 25193
SPELL_ID_CURSE_OF_TONGUES_3 = 25195
^^ astea sunt toate spelluri care sunt utilizate la encoutnerul de Ossirian.Le-am pus aici ca data gathering.
*/

#define SPELL_WARSTOMP        25188
#define SPELL_CURSEOFTONGUES         25195

struct MANGOS_DLL_DECL boss_ossirianAI : public ScriptedAI
{
    boss_ossirianAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 WarStomp_Timer;
    uint32 CurseOfTongues_Timer;
	bool InCombat;

    void Reset()
    {
        WarStomp_Timer = 10000;      //These times are probably wrong
        CurseOfTongues_Timer = 5000;
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
             //WarStomp_Timer
            if (WarStomp_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_WARSTOMP);

                //20 seconds until we should cast this agian
                WarStomp_Timer = 10000;
            }else WarStomp_Timer -= diff;

            //CurseOfTongues_Timer
            if (CurseOfTongues_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CURSEOFTONGUES);

                //15 seconds until we should cast this agian
                CurseOfTongues_Timer = 15000;
            }else CurseOfTongues_Timer -= diff;

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
CreatureAI* GetAI_boss_ossirian(Creature *_Creature)
{
    return new boss_ossirianAI (_Creature);
}


void AddSC_boss_ossirian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ossirian";
    newscript->GetAI = GetAI_boss_ossirian;
    m_scripts[nrscripts++] = newscript;
}