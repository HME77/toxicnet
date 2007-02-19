#include "../../sc_defines.h"

#define SPELL_SMASH       18944
#define SPELL_KNOCKAWAY         18945


struct MANGOS_DLL_DECL moltengiantAI : public ScriptedAI
{
    moltengiantAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 Smash_Timer;
    uint32 KnockAway_Timer;
	bool InCombat;

    void Reset()
    {
        pTarget = NULL;
        Smash_Timer = 10000;    
        KnockAway_Timer = 25000;
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
            
           
            if (Smash_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SMASH);                
                Smash_Timer = 10000;
            }else Smash_Timer -= diff;
           
            if (KnockAway_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_KNOCKAWAY);              
                KnockAway_Timer = 25000;
            }else KnockAway_Timer -= diff;
			


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
CreatureAI* GetAI_moltengiant(Creature *_Creature)
{
    return new moltengiantAI (_Creature);
}


void AddSC_moltengiant()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="moltengiant";
    newscript->GetAI = GetAI_moltengiant;
    m_scripts[nrscripts++] = newscript;
}