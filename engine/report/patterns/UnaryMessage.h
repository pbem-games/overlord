/***************************************************************************
                          UnaryMessage.h  -  description
                             -------------------
    begin                : Wed Jan 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef UNARY_PATTERN_H
#define UNARY_PATTERN_H

#include "ReportMessage.h"
class AbstractData;
class ReportPattern;
/**
  *@author Alex Dribin
  */

class UnaryMessage : public ReportMessage  {
public:
	UnaryMessage(){}
	UnaryMessage(ReportPattern * reporter, AbstractData * param1){reporter_ = reporter; param1_ = param1;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void printReport(ostream &out) const;
  void clean();
	static  UnaryMessage * headOfFreeList;
	union
	{
			UnaryMessage  * next; //for use in operator new
  		ReportPattern * reporter_;
	};
protected:
	~UnaryMessage(){}
		AbstractData * param1_;
};

#endif
