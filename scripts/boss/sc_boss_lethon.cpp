#include "../sc_defines.h"
//#define SPELL_SLEEP | cannot find spellid
//#define SPELL_MARKOFNATURE        25040 | under construction ;)
#define SPELL_SHADOWBOLTVOLLEY        27831
#define SPELL_CORRUPTIONOFEARTH         24910
#define SPELL_TAILSWEEP         15847
#define SPELL_NOXIOSBREATH 		24818

struct MANGOS_DLL_DECL boss_lethonAI : public ScriptedAI
{


    boss_lethonAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
	uint32 phase;
    uint32 SHADOWBOLTVOLLEY_Timer;
    uint32 TAILSWEEP_Timer;
    uint32 NOXIOSBREATH_Timer;
	bool InCombat;
	

    void Reset()
    {
        pTarget = NULL;
		phase = 1;
		SHADOWBOLTVOLLEY_Timer = 25000;
		TAILSWEEP_Timer = 15000;
		NOXIOSBREATH_Timer = 45000;
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

			if (TAILSWEEP_Timer < diff)
            {
				DoCast(m_creature->getVictim(),SPELL_TAILSWEEP);

                TAILSWEEP_Timer = 15000;
            
			}else TAILSWEEP_Timer -= diff;

            if (SHADOWBOLTVOLLEY_Timer < diff)
            {
				DoCast(m_creature,SPELL_SHADOWBOLTVOLLEY);
                
                SHADOWBOLTVOLLEY_Timer = 25000;

            }else SHADOWBOLTVOLLEY_Timer -= diff;
			
            if (NOXIOSBREATH_Timer < diff)
            {
				DoCast(m_creature->getVictim(),SPELL_NOXIOSBREATH);

                NOXIOSBREATH_Timer = 45000;
            
			}else NOXIOSBREATH_Timer -= diff;

			 if ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 75 && phase==1)
            {
                 m_creature->RemoveAllAuras();
                 DoCast(m_creature,SPELL_CORRUPTIONOFEARTH);
				 phase=2;
   
            }
 
			if ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 50  && phase==2)
			{
				m_creature->RemoveAllAuras();
				DoCast(m_creature,SPELL_CORRUPTIONOFEARTH);
				phase=3;
			}

			if ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 25  && phase==3)
			{
				m_creature->RemoveAllAuras();
				DoCast(m_creature,SPELL_CORRUPTIONOFEARTH);
				phase++;
			}

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
CreatureAI* GetAI_boss_lethon(Creature *_Creature)
{
    return new boss_lethonAI (_Creature);
}


void AddSC_boss_lethon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lethon";
    newscript->GetAI = GetAI_boss_lethon;
    m_scripts[nrscripts++] = newscript;
}