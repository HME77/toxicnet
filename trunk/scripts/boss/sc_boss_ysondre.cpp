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

// Made by Blackstar

#include "../sc_defines.h"
//#define SPELL_SUMMONDRUIDS	cant find right ID
#define SPELL_MARKOFNATURE	25040 //cast this on death of party member, doesnt work =(
#define SPELL_TAILSWEEP		15847
#define SPELL_LIGHTNIGHTWAVE	24819
#define SPELL_FEAR		33547
#define SPELL_NOXIUSBREATH	24818
//#define SPELL_SLEEP   cant find right ID

struct MANGOS_DLL_DECL boss_ysondreAI : public ScriptedAI
{


    boss_ysondreAI(Creature *c) : ScriptedAI(c) {Reset();}

	Unit *pTarget;
	uint32 phase;
	uint32 NOXIUSBREATH_Timer;
	uint32 LIGHTNIGHTWAVE_Timer;	
	uint32 TAILSWEEP_Timer;
	bool InCombat;
    

    void Reset()
    {
	phase=1;
	pTarget = NULL;
    TAILSWEEP_Timer = 15000; 
	NOXIUSBREATH_Timer = 45000;     
	LIGHTNIGHTWAVE_Timer = 60000;
	InCombat = false;		

        if (m_creature->isAlive())
        {	EnterEvadeMode();	}
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

            if (TAILSWEEP_Timer < diff)
            {
              if (!m_creature->HasInArc( M_PI, m_creature->getVictim()))
              DoCast(m_creature->getVictim(),SPELL_TAILSWEEP);
               TAILSWEEP_Timer = 15000;
            }else TAILSWEEP_Timer -= diff;

            if (LIGHTNIGHTWAVE_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_LIGHTNIGHTWAVE);

               LIGHTNIGHTWAVE_Timer = 60000;
            }else LIGHTNIGHTWAVE_Timer -= diff;    

			if (NOXIUSBREATH_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_NOXIUSBREATH);

               NOXIUSBREATH_Timer = 45000;
            }else NOXIUSBREATH_Timer -= diff; 
		
		/* Enable this when right ID is found for spells
		if ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 75 && phase==1)
            {
		m_creature->RemoveAllAuras();
            DoCast(m_creature,SPELL_SUMMONDRUIDS);
		phase=2;
		}
	
		if ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 50  && phase==2)
		{
		m_creature->RemoveAllAuras();
		DoCast(m_creature,SPELL_SUMMONDRUIDS);
		phase=3;
		}

		if ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 25  && phase==3)
		{
		m_creature->RemoveAllAuras();
		DoCast(m_creature,SPELL_SUMMONDRUIDS);
		phase++;
		}	*/	



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
CreatureAI* GetAI_boss_ysondre(Creature *_Creature)
{
    return new boss_ysondreAI (_Creature);
}


void AddSC_boss_ysondre()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ysondre";
    newscript->GetAI = GetAI_boss_ysondre;
    m_scripts[nrscripts++] = newscript;
}

