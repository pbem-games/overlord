/***************************************************************************
                          RecruitOrder.h
                             -------------------
    begin                : Thu Jun 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef RECRUIT_ORDER_H
#define RECRUIT_ORDER_H

#include "OrderPrototype.h"
class Reporter;

/**
  *@author Alex Dribin
  */

class RecruitOrder : public OrderPrototype  {
public: 
	RecruitOrder();
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
  ORDER_STATUS completeProcessing (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId, int result);
    protected:
static const  UINT INVALID_RECRUIT_REPORT_FLAG;
};

#endif
