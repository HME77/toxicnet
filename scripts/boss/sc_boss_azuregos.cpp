#include "../sc_defines.h"

#define SPELL_FROSTBREATH			22479
#define SPELL_CLEAVE				20691
#define SPELL_MANASTORM				21097
//#define SPELL_REFLECTIONSHIELD		10831
//#define SPELL_MARKOFFROST			23182 (Under construction ^^)
//#define SPELL_TELEPORT				16807

struct MANGOS_DLL_DECL boss_azuregosAI : public ScriptedAI
{
    boss_azuregosAI(Creature *c) : ScriptedAI(c) {Reset();}
    
    uint32 FROSTBREATH_Timer;
    uint32 CLEAVE_Timer;
	uint32 MANASTORM_Timer;
  //  uint32 REFLECTIONSHIELD_Timer;
    //uint32 TELEPORT_Timer;
    
	Unit *pTarget;
	bool InCombat;


    void Reset()
    {
        pTarget = NULL;
		FROSTBREATH_Timer = 25000;
		CLEAVE_Timer = 15000;
		MANASTORM_Timer = 35000;
	//	REFLECTIONSHIELD_Timer = 45000;
		//TELEPORT_Timer = 30000;
		InCombat = false;

    if (m_creature->isAlive())
    {    EnterEvadeMode();    }
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

            if (FROSTBREATH_Timer < diff)
            {
				DoCast(m_creature,SPELL_FROSTBREATH);
                
                FROSTBREATH_Timer = 25000;

            }else FROSTBREATH_Timer -= diff;
			
            if (CLEAVE_Timer < diff)
            {
				DoCast(m_creature->getVictim(),SPELL_CLEAVE);

                CLEAVE_Timer = 15000;
            
			}else CLEAVE_Timer -= diff;

			if (MANASTORM_Timer < diff)
            {
				DoCast(m_creature,SPELL_MANASTORM);
                
                MANASTORM_Timer = 35000;

            }else MANASTORM_Timer -= diff;
			
  /*          if (REFLECTIONSHIELD_Timer < diff)
            {
				DoCast(m_creature->getVictim(),SPELL_REFLECTIONSHIELD);

                REFLECTIONSHIELD_Timer = 45000;
            
			}else REFLECTIONSHIELD_Timer -= diff;

			if (TELEPORT_Timer < diff)
            {
				DoCast(m_creature->getVictim(),SPELL_TELEPORT);

                TELEPORT_Timer = 30000;
            
			}else TELEPORT_Timer -= diff;
*/
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
CreatureAI* GetAI_boss_azuregos(Creature *_Creature)
{
    return new boss_azuregosAI (_Creature);
}


void AddSC_boss_azuregos()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_azuregos";
    newscript->GetAI = GetAI_boss_azuregos;
    m_scripts[nrscripts++] = newscript;
}