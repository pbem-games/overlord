/***************************************************************************
                          CombatTactics.h  -  description
                             -------------------
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef COMBAT_TACTICS_H
#define COMBAT_TACTICS_H
#include <iostream>
#include <string>

class CombatMoveVariety;
class CombatStanceVariety;
class CombatRankVariety;
class CombatFileVariety;
class Parser;
 
/**
  *@author Alex Dribin
  */

class CombatTactics {
public: 
	CombatTactics();
	~CombatTactics(){}
	CombatTactics(CombatTactics & p);
   STATUS     initialize      ( Parser *parser);
   void     defaultInitialization();
   void       save (ostream &out);
   void       save (ostream &out, string prefix);
   void       report (ostream &out);
   string print();
    inline CombatMoveVariety * getCombatMove() const {return move_;}
    inline CombatStanceVariety * getCombatStance() const {return stance_;}
    inline CombatRankVariety * getCombatRank() const {return rank_;}
    inline CombatFileVariety * getCombatFile() const {return file_;}
    inline void setCombatMove(CombatMoveVariety * value) { move_ = value;}
    inline void setCombatStance(CombatStanceVariety * value) { stance_ = value;}
    inline void setCombatRank(CombatRankVariety * value) { rank_ = value;}
    inline void setCombatFile(CombatFileVariety * value) { file_ = value;}
    bool isDefined();
    bool isDefault();
    void undefine();
    inline bool isUnknown(){return isUnknown_;}
    protected:
    CombatMoveVariety * move_;
    CombatStanceVariety * stance_;
    CombatRankVariety   * rank_;
    CombatFileVariety   * file_;
    bool isUnknown_;
    private:
};
   inline ostream& operator << (ostream& out,  CombatTactics& tactics)
                                { out<<tactics.print(); return out;}
#endif
