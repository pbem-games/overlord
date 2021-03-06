/***************************************************************************
                             ReshowOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ReshowOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "FactionEntity.h"
#include "TokenEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "RuleIndex.h"

extern ReportPattern * ReshowReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	invalidParameterReporter;
ReshowOrder * instantiateReshowOrder = new ReshowOrder();

ReshowOrder::ReshowOrder(){
  keyword_ = "Reshow";
  registerOrder_();
  description = string("RESHOW rule-keyword | rule-tag | skill-tag [skill-level] | [ALL] \n") +
  "Includes in your report the  descriptions for all rules designated by keyword\n"+
  "or if specific rule-tag provided descriptions for the designated rule will be reported.\n" +
  "If this rule is skill-rule descriptions for the designated skill from\n" +
  "the specified skill level up to the maximum skill limit you ever reached.\n" +
  "will be included. RESHOW ALL will report all knowledge about all rules\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS ReshowOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity,NO_PARSING_REPORT) && !entityIsFaction(entity,NO_PARSING_REPORT))
            return IO_ERROR;


	string tag = parser->getWord();
   if (tag.size() == 0)  // Missing parameter
        {
        entity->addReport(new BinaryMessage(missingParameterReporter, new StringData(keyword_), new StringData("rule tag")));
         return IO_ERROR;
        }
   //  [ALL]
        if(!ciStringCompare(tag.c_str(),string("ALL")))
        {
          parameters.push_back(new StringData(tag));
          return OK;
        }
// if this is a keyword?
  BasicRulesCollection  * collection = gameFacade->ruleIndex.findRuleCollection(tag);
  if(collection != 0)
  {
    parameters.push_back(new StringData(tag));
    return OK;
  }
// Check tag for being rule-tag or skill-tag
   Rule * rule = gameFacade->ruleIndex.findRule(tag);
   if(rule==0)
    {

      entity->addReport(new TertiaryMessage(invalidParameterReporter, new StringData(keyword_), new StringData(tag), new StringData("rule tag")));
      return IO_ERROR;
    }
    parameters.push_back(rule);
// If it is a skilltag - read also optional level parameter
	if(dynamic_cast<SkillRule*>(rule))
			parseIntegerParameter(parser, parameters);

    return OK;

}



ORDER_STATUS ReshowOrder::process (Entity * entity, ParameterList &parameters)
{
  FactionEntity * faction = dynamic_cast<FactionEntity *>(entity);
  if(faction == 0)
  {
    TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
    assert(unit);
    faction = unit->getFaction();
  }

  StringData * par = dynamic_cast<StringData *>(parameters[0]);


  if(par)
  {
    string tag = par->print();
  //  [ALL]
   if(!ciStringCompare(tag.c_str(),string("ALL")))
    {
      for(vector < BasicRulesCollection  *>::iterator collIter = gameFacade->ruleIndex.getAllRules().begin();
          collIter != gameFacade->ruleIndex.getAllRules().end(); ++collIter)
      {
          faction->markCollectionToReshow(*collIter);
      }
    }
    else
    {
    BasicRulesCollection  * collection = gameFacade->ruleIndex.findRuleCollection(tag);
	if(collection==0)
	{
	    cerr << "Unknown collection "<< tag<<endl;
		return FAILURE;
	}
    faction->markCollectionToReshow(collection);
    }
    return SUCCESS;
  }
  Rule * rule = dynamic_cast<Rule *>(parameters[0]);
  assert (rule);
  SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]);
  if(skill)
    {
        int level = getIntegerParameter(parameters,1);
        faction->markSkillToReshow(skill,level);
    }
  else
  {
    faction->markKnowledgeToReshow(rule);
  }
    return SUCCESS;
}
// If all rule collection or specific rule needs to be reshown add coll or rule
// to special vector.

