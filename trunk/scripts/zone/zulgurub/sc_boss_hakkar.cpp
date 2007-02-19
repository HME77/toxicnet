
#include "../../sc_defines.h"
#define SPELL_CORRUPTEDBLOOD        24328
#define SPELL_BLOODSIPHON        24323
//#define SPELL_CAUSEINSANITY        12888
#define SPELL_ENRAGE        24318



struct MANGOS_DLL_DECL boss_hakkarAI : public ScriptedAI
{
    boss_hakkarAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 CorruptedBlood_Timer;
    uint32 BloodSiphon_Timer;
    uint32 CauseInsanity_Timer;
    uint32 Enrage_Timer;
	bool InCombat;

    void Reset()
    {
        pTarget = NULL;
        CorruptedBlood_Timer = 15000;      //These times are probably wrong
        BloodSiphon_Timer = 90000;
        //CauseInsanity_Timer = 15000;
        Enrage_Timer = 600000;
		InCombat = false;
		
        

    if (m_creature->isAlive())
      {		EnterEvadeMode();	}
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
        if (!who)
            return;

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            if ( m_creature->getVictim() == NULL)
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
            
            //CorruptedBlood_Timer
            if (CorruptedBlood_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CORRUPTEDBLOOD);

                //25 seconds until we should cast this agian
                CorruptedBlood_Timer = 25000;
            }else CorruptedBlood_Timer -= diff;

            //BloodSiphon_Timer
            if (BloodSiphon_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BLOODSIPHON);

                //90 seconds until we should cast this agian
                BloodSiphon_Timer = 90000;
            }else BloodSiphon_Timer -= diff;
            
            //CauseInsanity_Timer
            
           /*

           if (CauseInsanity_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CAUSEINSANITY);

                //15 seconds until we should cast this agian
                CauseInsanity_Timer = 15000;
            }else CauseInsanity_Timer -= diff;
            //Enrage_Timer
            
            */
            if (Enrage_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_ENRAGE);

                // 10 minutes till first cast :)
                Enrage_Timer = 600000;
            }else Enrage_Timer -= diff;

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
            
            //If we are still alive and we lose our victim it means we killed them
            if(m_creature->isAlive() && !m_creature->getVictim())
            {
                Reset();
            }
        }
    }
};
CreatureAI* GetAI_boss_hakkar(Creature *_Creature)
{
    return new boss_hakkarAI (_Creature);
}


void AddSC_boss_hakkar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_hakkar";
    newscript->GetAI = GetAI_boss_hakkar;
    m_scripts[nrscripts++] = newscript;
}
