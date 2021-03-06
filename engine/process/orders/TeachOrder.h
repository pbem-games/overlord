/***************************************************************************
                          TeachOrder.h
                             -------------------
    begin                : Thu Jul 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef TEACH_ORDER_H
#define TEACH_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class TeachOrder : public OrderPrototype  {
public:
	TeachOrder();
	~TeachOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
   void preProcess_(Entity * entity, ParameterList &parameters);
   void doProcess_(Entity * entity, ParameterList &parameters);
};

#endif
