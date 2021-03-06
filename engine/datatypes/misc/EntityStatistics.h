/***************************************************************************
                          EntityStatistics.h
                             -------------------
    begin                : Wed Feb 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef ENTITY_STATISTICS_H
#define ENTITY_STATISTICS_H
#include <iostream>
#include "LineParser.h"
#include "OverlordTypes.h"
using namespace std;

/**Set of characteristics entity may have
  *@author Alex Dribin
  */

class EntityStatistics {
public:
	EntityStatistics();
	~EntityStatistics(){}

  STATUS initialize(Parser * parser);
  inline int getInitiative() const {return initiative_;}
  inline int getMelee() const {return melee_;}
  inline int getDefence() const {return defence_;}
  inline int getDamage() const {return damage_;}
  inline int getRangedDamage() const {return rangedDamage_;}
  inline DAMAGE_TYPE getDamageType() const {return damageType_;}
  inline int getHits() const {return numOfHits_;}
  inline int getMissile() const {return missile_;}
  inline int getLife() const {return life_;}
  inline int getMana() const {return mana_;}
  inline int getUpkeep() const {return upkeep_;}
  inline int getControlPoints() const {return controlPoints_;}
  inline int getStealth() const {return stealth_;}
  inline int getObservation() const {return observation_;}
  void scaleControlPoints(int num, int factor);

  inline void modifyInitiative(int initiative) {initiative_ += initiative;}
  inline void modifyMelee(int melee) {melee_ += melee;}
  inline void modifyDefence(int defence) {defence_ += defence;}
  inline void modifyDamage(int damage) {damage_ += damage;}
  inline void modifyRangedDamage(int damage) {rangedDamage_ += damage;}
         void modifyDamageType(DAMAGE_TYPE damageType);
  inline void modifyHits(int numOfHits) {numOfHits_ += numOfHits;}
  inline void modifyMissile(int missile) {missile_ += missile;}
  inline void modifyLife(int life) {life_ += life;}
  inline void modifyMana(int mana) {mana_ += mana;}
  inline void modifyUpkeep(int upkeep) {upkeep_ += upkeep;}
  inline void modifyControlPoints(int controlPoints) {controlPoints_ += controlPoints;}
  inline void modifyStealth(int stealth) {stealth_ += stealth;}
  inline void modifyObservation(int observation) {observation_ += observation;}
	inline void setInitiative(int initiative) {initiative_ = initiative;}
        void reportStatistics(ostream &out);
        void addStats(EntityStatistics * stats);
        void setStats(EntityStatistics * stats);
        void replaceStats(EntityStatistics * stats);
        void addPartialStats(EntityStatistics * stats, int numenator, int denominator);
        void clearStats(int value =0);
        bool empty();
        void save(ostream &out,string prefix, int defaultValue);
        vector<AbstractArray> aPrint();
        void aPrintThisStat_(int thisStat, const char * name, vector<AbstractArray> & out );
    protected:
	int initiative_;
	int melee_;
	int defence_;
	int damage_;
	int rangedDamage_;
	DAMAGE_TYPE damageType_;
	int numOfHits_;
	int missile_;
	int life_;
	int mana_;
	int upkeep_;
	int controlPoints_;
	int stealth_;
	int observation_;
	void printThisStat_(int thisStat, const char * name, bool & isFirst, ostream &out);
};

   inline ostream& operator << (ostream& out,  EntityStatistics& data)
                                { data.reportStatistics(out); return out;}

#endif
