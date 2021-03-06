/***************************************************************************
                          ReportPattern.h  -  description
                             -------------------
    begin                : Wed Jan 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef REPORTER_H
#define REPORTER_H
#include <string>
#include <vector>
#include <iostream>
#include "OverlordTypes.h"
#include "ReportPrinter.h"
using namespace std;
class AbstractData;

class ReportPattern {
public:
	ReportPattern(const string & keytag);
	void printReport (ReportPrinter &out);
	void printReport (ReportPrinter &out, AbstractData * param1);
	void printReport (ReportPrinter &out, AbstractData * param1, AbstractData * param2);
	void printReport (ReportPrinter &out, AbstractData * param1, AbstractData * param2, AbstractData * param3);
	void printReport (ReportPrinter &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4);
	void printReport (ReportPrinter &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4, AbstractData * param5);
  inline string & getKeytag(){return keytag_;}
  //static vector<ReportPattern *> reportPatternsRegistry;
  static STATUS initialize(const string & fileName);
  static const string keyword;
  
    vector <AbstractData *>  aPrint();
    vector <AbstractData *>  aPrint( AbstractData * param1);
    vector <AbstractData *>  aPrint( AbstractData * param1, AbstractData * param2);
    vector <AbstractData *>  aPrint( AbstractData * param1, AbstractData * param2, AbstractData * param3);
    vector <AbstractData *>  aPrint( AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4);
    vector <AbstractData *>  aPrint( AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4, AbstractData * param5);
  
	protected:
  string keytag_;
	~ReportPattern(){}
	string rep1_;
	string rep2_;
	string rep3_;
	string rep4_;
	string rep5_;
	string rep6_;
	private:
};
//extern vector<ReportPattern *> * reportPatternsRegistry;

#endif


