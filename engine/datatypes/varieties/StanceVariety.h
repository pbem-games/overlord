/***************************************************************************
                          StanceVariety.h  -  description
                             -------------------
    begin                : Thu Jan 9 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef STANCE_H
#define STANCE_H

#include <iostream>
#include "Variety.h"
#include "VarietiesCollection.h"
using namespace std;



class StanceVariety : public Variety  {
public:
      StanceVariety ( const string & keyword, GameData * parent): Variety(keyword, parent){}
      StanceVariety ( const StanceVariety * prototype ): Variety(prototype){code_ = 0;}
      ~StanceVariety (){}
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      bool operator <   (StanceVariety stance2);  
      bool operator <=  (StanceVariety stance2); 
      bool operator ==  (StanceVariety stance2);
      bool operator >=  (StanceVariety stance2);
      bool operator >   (StanceVariety stance2);
       
  /** No descriptions */
  		inline int getCode(){return code_;}
friend  ostream &operator << ( ostream &out, StanceVariety * stance);
    protected:
			int code_;
    private:
};
extern StanceVariety     sampleStance;
extern StanceVariety *  privateStance;
extern StanceVariety *  alliedStance;
extern StanceVariety *  friendlyStance;
extern StanceVariety *  neutralStance;
extern StanceVariety *  unknownStance;
extern StanceVariety *  hostileStance;
extern StanceVariety *  enemyStance;
//extern VarietiesCollection <StanceVariety>    stances;
extern StanceVariety    sampleStance;
#endif
