/***************************************************************************
                          ReshowOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef RESHOW_ORDER_H
#define RESHOW_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class ReshowOrder : public OrderPrototype  {
public:
	ReshowOrder();
	~ReshowOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
