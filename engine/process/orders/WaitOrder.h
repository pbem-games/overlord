/***************************************************************************
                          WaitOrder.h 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef WAIT_ORDER_H
#define WAIT_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class Reporter;
class AbstractData;

class WaitOrder : public OrderPrototype  {
public: 
	WaitOrder();
	~WaitOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters);
    protected:
};

#endif