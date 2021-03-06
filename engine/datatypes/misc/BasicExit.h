/***************************************************************************
                          BasicExit.h
                             -------------------
    begin                : Sun Jan 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef BASIC_EXIT_H
#define BASIC_EXIT_H
#include <iostream>
#include <vector>
#include "GameData.h"
#include "MovementElement.h"
#include "ReportPrinter.h"

using namespace std;
class LocationEntity;
class DirectionVariety;
//class MovementRule;
class UnitEntity;
class MovementVariety;
class Parser;

class BasicExit :public GameData{
public: 
	BasicExit(){};
	BasicExit(LocationEntity * origin, DirectionVariety * dir, LocationEntity * dest);
	virtual ~BasicExit(){};
  virtual void save(ostream & out);
  virtual string print();
  virtual void produceReport(ReportPrinter & out);
  // aPrint is a function that performs print() in a form convinient for interpretaion in GUI application 
  vector <AbstractData *> aPrint();
  virtual inline LocationEntity * getDestination() const {return destination_;}
  virtual inline DirectionVariety * getDirection() const {return dir_;}
  virtual int getTravelTime(MovementVariety * mode);
    protected:
    DirectionVariety * dir_;
		LocationEntity * destination_;
		LocationEntity * origin_;
   private:
};
typedef	vector <BasicExit *>::iterator DirectionIterator;
#endif
