#include "../../sc_defines.h"

#define SPELL_CLEAVE       19632
#define SPELL_CONE         19630
#define SPELL_ANCIENTDREAD       19365
#define SPELL_CAUTERIZINGFLAMES         19366
#define SPELL_WITHERINGHEAT       19367
#define SPELL_ANCIENTDESPAIR         19369
#define SPELL_ANCIENTHYSTERIA         19372
#define SPELL_GROUNDSTOMP         19364

struct MANGOS_DLL_DECL ancientcorehoundAI : public ScriptedAI
{
    ancientcorehoundAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 Cleave_Timer;
    uint32 Cone_Timer;
	uint32 RandomAbility_Timer;
	uint32 i;
	bool InCombat;		

    void Reset()
    {
        pTarget = NULL;
        Cleave_Timer = 10000;    
        Cone_Timer = 25000;
		RandomAbility_Timer = 15000;
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
            
           
            if (Cleave_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CLEAVE);                
                Cleave_Timer = 10000;
            }else Cleave_Timer -= diff;
           
            if (Cone_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CONE);              
                Cone_Timer = 25000;
            }else Cone_Timer -= diff;
			
			srand ( time(NULL) );
			i = rand() % 6 + 1;

			switch(i) {
			
			case 1:			
				if (RandomAbility_Timer < diff)
				{
					//Cast
					DoCast(m_creature,SPELL_ANCIENTDREAD);              
					RandomAbility_Timer = 15000;
				}else RandomAbility_Timer -= diff;
			break;
			case 2:			
				if (RandomAbility_Timer < diff)
				{
					//Cast
					DoCast(m_creature->getVictim(),SPELL_CAUTERIZINGFLAMES);              
					RandomAbility_Timer = 15000;
				}else RandomAbility_Timer -= diff;
			break;
			case 3:			
				if (RandomAbility_Timer < diff)
				{
					//Cast
					DoCast(m_creature->getVictim(),SPELL_WITHERINGHEAT);              
					RandomAbility_Timer = 15000;
				}else RandomAbility_Timer -= diff;
			break;
			case 4:			
				if (RandomAbility_Timer < diff)
				{
					//Cast
					DoCast(m_creature->getVictim(),SPELL_ANCIENTDESPAIR);              
					RandomAbility_Timer = 15000;
				}else RandomAbility_Timer -= diff;
			break;
			case 5:			
				if (RandomAbility_Timer < diff)
				{
					//Cast
					DoCast(m_creature->getVictim(),SPELL_ANCIENTHYSTERIA);              
					RandomAbility_Timer = 15000;
				}else RandomAbility_Timer -= diff;
			break;
			case 6:			
				if (RandomAbility_Timer < diff)
				{
					//Cast
					DoCast(m_creature->getVictim(),SPELL_GROUNDSTOMP);              
					RandomAbility_Timer = 15000;
				}else RandomAbility_Timer -= diff;
			break;
			}


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
CreatureAI* GetAI_ancientcorehound(Creature *_Creature)
{
    return new ancientcorehoundAI (_Creature);
}


void AddSC_ancientcorehound()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="ancientcorehound";
    newscript->GetAI = GetAI_ancientcorehound;
    m_scripts[nrscripts++] = newscript;
}