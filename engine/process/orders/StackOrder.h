/***************************************************************************
                          StackOrder.h
                             -------------------
    begin                : Mon Mar 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef STACK_ORDER_H
#define STACK_ORDER_H

#include "OrderPrototype.h"
class ReportPattern;

/**
  *@author Alex Dribin
  */

class StackOrder : public OrderPrototype  {
public:
	        StackOrder();
	        ~StackOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
