#include "../sc_defines.h"

#define SPELL_TEARSOFTHEWINDSEEKER		23011
#define SPELL_THENDRILSOFAIR         23009

struct MANGOS_DLL_DECL boss_thunderaanAI : public ScriptedAI
{
    boss_thunderaanAI(Creature *c) : ScriptedAI(c) {Reset();}
    
    uint32 TEARSOFTHEWINDSEEKER_Timer;
    uint32 THENDRILSOFAIR_Timer;
	bool InCombat;    
	Unit *pTarget;


    void Reset()
    {
        pTarget = NULL;
		TEARSOFTHEWINDSEEKER_Timer =  25000;	//These times are probably wrong
        THENDRILSOFAIR_Timer = 15000; 
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


        if( m_creature->getVictim() && m_creature->isAlive())
        {

            if (needToStop())
            {
                Reset();
                return;
            }

            if (TEARSOFTHEWINDSEEKER_Timer < diff)
            {
				DoCast(m_creature,SPELL_TEARSOFTHEWINDSEEKER);
                
                TEARSOFTHEWINDSEEKER_Timer = 25000;

            }else TEARSOFTHEWINDSEEKER_Timer -= diff;
			
            if (THENDRILSOFAIR_Timer < diff)
            {
				DoCast(m_creature->getVictim(),SPELL_THENDRILSOFAIR );

                THENDRILSOFAIR_Timer = 15000;
            
			}else THENDRILSOFAIR_Timer -= diff;

            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    

                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
            
            
        }
    }
};
CreatureAI* GetAI_boss_thunderaan(Creature *_Creature)
{
    return new boss_thunderaanAI (_Creature);
}


void AddSC_boss_thunderaan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_thunderaan";
    newscript->GetAI = GetAI_boss_thunderaan;
    m_scripts[nrscripts++] = newscript;
}