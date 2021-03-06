/***************************************************************************
                          EnchantCombatAction.cpp
                             -------------------
    begin                : 23 apr 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "EnchantCombatAction.h"
#include "TokenEntity.h"
#include "CombatEngine.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "reporting.h"
#include "PrototypeManager.h"
#include "CombatAttackMessage.h"
#include "CombatReport.h"
#include "EnchantmentRule.h"
#include "CombatTargetVariety.h"
#include "CombatOrderLine.h"
extern int Roll_1Dx(int n);
extern ReportPattern * noTargetsReporter;
extern ReportPattern * combatCastReporter;
EnchantCombatAction     sampleEnchantAction     ("COMBAT_ENCHANT",  &sampleCombatAction);

GameData * EnchantCombatAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<EnchantCombatAction> (this);
}



STATUS
EnchantCombatAction::initialize        ( Parser *parser )
{
  CombatActionStrategy::initialize(parser);
  
  if (parser->matchKeyword ("GRANTS") )
  {
    enchant_ = gameFacade->enchantments[parser->getWord()];
    duration_ =  parser->getInteger();
    
    if(duration_ == 0)
      duration_ = 1; // (default)
    if( enchant_ == 0 )
    {
      cout << "Error while reading GRANTS \n";
      return IO_ERROR;
    }
    return OK;
  }
  if (parser->matchKeyword ("SECONDARY_ACTION") )
  {
    string keyword = parser->getWord();
    if(keyword =="NEXT")
    {
        nextCombatAction_ = true;
        return OK;
    }

    GameData * temp =  prototypeManager->findInRegistry(keyword);
    if(temp == 0)
    {
      cout << "Unknown secondary combat action " << keyword  << " for skill "
      << print()<< endl;
    }
    else
    {
      secondaryCombatAction_ =
      dynamic_cast<CombatActionStrategy *>(temp ->createInstanceOfSelf ());
      
    }
    return OK;
  }
  return OK;
  
}

void EnchantCombatAction::save(ostream &out)
{
    CombatActionStrategy::save(out);
    if(target_) out<<"COMBAT "<< "TARGET"<<" "<<target_->getTag() <<endl;
    //if(action_)out<<"COMBAT "<< "EFFECT"<<" "<<action_->getTag() <<endl;
    if(range_)out<<"COMBAT "<< "RANGE"<<" "<<range_ <<endl;
    if(mana_)out<<"COMBAT "<<"USE_MANA"<<" "<< mana_<<endl;
    for(vector <ItemElement *>::iterator iter =resources_.begin(); iter != resources_.end();++iter)
    {
      out<<"COMBAT "<<"CONSUME ";
      (*iter)->save(out);
    }
    modifyingStats.save(out,"COMBAT BONUS ",0);
    nonCumulativeStats.save(out,"COMBAT ",VERY_BIG_NUMBER);
}

// get all potential targets
BattleTargets EnchantCombatAction::getPotentialTargets(BattleInstance * battleInstance, CombatReport * report)
{
  BattleTargets potentialTargets;
  BattleField * battleField = battleInstance->getBattleField();
  //	CombatReport * report = battleField->getCombatEngine()->getCombatReport();
  
  if(target_ == battleTargetSelf)
  {
    potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));
  }
  
  else if (target_ == battleTargetAll)// Dummy. Target should not be empty
  {
    potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));
  }
  
  else if (target_ == battleTargetOwnSide)// Dummy. Target should not be empty
  {
    potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));
  }
  
  else if (target_ == battleTargetOppositeSide)// Dummy. Target should not be empty
  {
    potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));;
  }
  
  else if (target_ == battleTargetOpposing)
  {
    battleField->addAllEnemiesAtRange(battleInstance, potentialTargets, range_);
  }
  
  else if (target_ == battleTargetFriend)
  {
    battleField->addAllFriendsAtRange(battleInstance, potentialTargets, range_);
  }
  
  else if (target_ == battleTargetFriendlyLeader)
  {
    battleField->addAllFriendsAtRange(battleInstance, potentialTargets, range_);
    // Now erase all non-leaders
    for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
    iter != potentialTargets.end(); )
    {
      if(! (*iter).instance_->isLeader())
      {
	potentialTargets.erase(iter);
	continue;
      }
      ++iter;
    }
  }
  
  else if (target_ == battleTargetOpposingLeader)
  {
    battleField->addAllEnemiesAtRange(battleInstance, potentialTargets, range_);
    // Now erase all non-leaders
    for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
    iter != potentialTargets.end(); )
    {
      if(! (*iter).instance_->isLeader())
      {
	potentialTargets.erase(iter);
	continue;
      }
      ++iter;
    }
  }
  
  else
  {
    //cerr << target_->
  }
  
  
  
  
  
  
  
  //	if(potentialTargets.empty()) // No targets. Report
  //	{
    //		return potentialTargets;
    //	}
    return potentialTargets;
    
    
}



void	EnchantCombatAction::performAction(BattleInstance * battleInstance, BattleTargets & potentialTargets, CombatReport * report)
{
  if(!battleInstance->isProcessingRepeatingOrder()) // Enchantment is added only once
    // per round
    {
      report->add(new BinaryMessage(combatCastReporter, battleInstance->getOrigin(), new StringData(enchant_->getName()))); // do not need tag of enchantment
      if(target_ == battleTargetSelf)
      {
	battleInstance->addEnchantment(new EnchantmentElement(enchant_, duration_));
      }
      else if (target_ == battleTargetAll)
      {
	battleInstance->getBattleField()->getCombatEngine()
	->addAttackerSideEnchantment(new EnchantmentElement(enchant_, duration_));
	battleInstance->getBattleField()->getCombatEngine()
	->addDefenderSideEnchantment(new EnchantmentElement(enchant_, duration_));
	
      }
      else if (target_ == battleTargetOwnSide)
      {
	if(battleInstance->isAttacker())
	  battleInstance->getBattleField()->getCombatEngine()
	  ->addAttackerSideEnchantment(new EnchantmentElement(enchant_, duration_));
	else
	  battleInstance->getBattleField()->getCombatEngine()
	  ->addDefenderSideEnchantment(new EnchantmentElement(enchant_, duration_));
	
      }
      else if (target_ == battleTargetOppositeSide)
      {
	if(battleInstance->isAttacker())
	  battleInstance->getBattleField()->getCombatEngine()
	  ->addDefenderSideEnchantment(new EnchantmentElement(enchant_, duration_));
	else
	  battleInstance->getBattleField()->getCombatEngine()
	  ->addAttackerSideEnchantment(new EnchantmentElement(enchant_, duration_));
      }
      else
      {
	//cerr << target_->
      }
      
      
      
      
      
    }
    if(secondaryCombatAction_)
    {
      secondaryCombatAction_->performAction(battleInstance,  potentialTargets, report);
    }

    if(nextCombatAction_)
    {
      //battleInstance->getCurrentOrder()->setNexOrderToBeExecuted(true);
      //battleInstance->setActedOnRound(false);
      battleInstance->setOneMoreActionOnRound(true);
    }
    
}


