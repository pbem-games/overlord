/***************************************************************************
                          SkillCondition.h  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef SKILL_CONDITION_H
#define SKILL_CONDITION_H
#include "BasicCondition.h"
#include "SkillLevelElement.h"
class UnitEntity;
class ItemRule;
class Rule;

//class SkillLevelElement;

class SkillCondition : public BasicCondition  {
public: 
  SkillCondition(const string & keyword, GameData * parent ): BasicCondition(keyword, parent){}
  SkillCondition(const SkillCondition * prototype);
	~SkillCondition(){delete skillRequirement_;}
  STATUS      initialize ( Parser *parser );
  virtual void save(ostream &out);
  GameData * createInstanceOfSelf();
  bool isSatisfied(TokenEntity * unit, Entity * target=0 );
//  void conditionHandler(Entity * entity);
//  void setSubject(Rule * subject);
  void extractKnowledge (Entity * recipient, int parameter = 0);
  SkillLevelElement * getSkillLevel() const {return skillRequirement_;}
//  Rule * getSubject() const ;
  SkillRule * getSkill() const ;
  int  getLevel() const ;
//  ostream & reportCondition(ostream & out);
  string print(){return string("condition: ") + skillRequirement_->print();}
  vector <AbstractData *> aPrint();
protected:
	SkillLevelElement * skillRequirement_;
//  ItemRule * subject_;
};
extern SkillCondition  sampleSkillCondition;

#endif
