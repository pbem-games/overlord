/***************************************************************************
                             DayOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "DayOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern int currentDay;
extern Reporter *	invalidParameterReporter;

DayOrder * instantiateDayOrder = new DayOrder();

DayOrder::DayOrder(){
  keyword_ = "Day";
  registerOrder_();
  description = string("DAY number \n") +
  "Immediate, condition, one-shot.  This order executes and completes on the given\n" +
  "day of the month.\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS DayOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseIntegerParameter(parser, parameters))
    {

      entity->addReport(new TertiaryPattern(invalidParameterReporter, new StringData(keyword_), new StringData(""), new StringData("integer number")));
      return IO_ERROR;
    }
  return OK;

}



ORDER_STATUS DayOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
	if (currentDay == getIntegerParameter(parameters,0))
		return SUCCESS;
	else	
		return FAILURE;
}
