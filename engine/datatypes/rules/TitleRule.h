/***************************************************************************
                          Title.h  

                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef TITLE_H
#define TITLE_H
#include "Rule.h"
#include "GameData.h"
class LocationEntity;
class UnitEntity;
class SkillCondition;
class SkillLevelElement;
class BonusElement;

class TitleRule : public Rule
{
    public:
      TitleRule (const string & keyword, GameData * parent ): Rule(keyword, parent){}
			TitleRule (const TitleRule * prototype );
      void       printDescription(ReportPrinter & out);
      virtual    STATUS initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();

      int  getCost()    const  {  return cost_;}
      int  getControl() const  {  return control_;}
      int  getType()    const  {  return type_;}
      int  getRange()   const  {  return range_;}
      void  setCost        ( int cost)      {  cost_=cost;}
      void  setControl     ( int control) {  control_=control;}
      void  setType        ( int type)      {  type_=type;}
      void  setRange       ( int range)    {  range_=range;}
      inline SkillCondition *    getClaimingCondition()      const     {return condition_;}

      void extractKnowledge (Entity * recipient, int parameter = 0);
      SkillLevelElement * getLearningLevelBonus() {return learningLevelBonus_;}
      BonusElement *      getStudyBonus() {return studyBonus_;}
      virtual bool contest(UnitEntity *  titleHolder, UnitEntity * contender,
                                          LocationEntity * location);
      AbstractData * getContestCriteria(UnitEntity * unit);                                    
      virtual void activateClaimingEffects(UnitEntity * titleHolder, LocationEntity * location);
      virtual void desactivateClaimingEffects(UnitEntity * titleHolder, LocationEntity * location);
      int markTerritoryOwned(LocationEntity * start,
                  UnitEntity * titleHolder, int distance );
    protected:
   int cost_;                  //cost to challenge for the title
   int control_;               //number of control points title added to faction
   int type_;                  //There are type 0 through type 3.
   int range_;                 //the number of hexes dominated by this title
   SkillCondition *  condition_;
   SkillLevelElement *  learningLevelBonus_;
   BonusElement * studyBonus_;
   private:
};
extern TitleRule      sampleTitle;

class MerchantPrinceTitleRule : public TitleRule
{
public:
   MerchantPrinceTitleRule (const string & keyword, GameData * parent ) : TitleRule(keyword, parent){}
   MerchantPrinceTitleRule (const MerchantPrinceTitleRule * prototype  ): TitleRule(prototype){}
   GameData * createInstanceOfSelf();
   void activateClaimingEffects(UnitEntity * titleHolder, LocationEntity * location);
   void desactivateClaimingEffects(UnitEntity * titleHolder, LocationEntity * location);
private:
};



class OverlordTitleRule : public TitleRule
{
public:
   OverlordTitleRule (const string & keyword, GameData * parent ) : TitleRule(keyword, parent){}
   OverlordTitleRule (const OverlordTitleRule * prototype   ) : TitleRule(prototype){}
   GameData * createInstanceOfSelf();
   bool contest(UnitEntity *  titleHolder, UnitEntity * contender,
                                          LocationEntity * location);
   void activateClaimingEffects(UnitEntity * titleHolder, LocationEntity * location);
   void desactivateClaimingEffects(UnitEntity * titleHolder, LocationEntity * location);
private:
};
#include "RulesCollection.h"
extern RulesCollection <TitleRule>     titles;

#endif