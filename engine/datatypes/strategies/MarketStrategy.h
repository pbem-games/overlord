/***************************************************************************
                          MarketStrategy.h
                             -------------------
    begin                : Tue Jun 24 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef MARKET_STRATEGY_H
#define MARKET_STRATEGY_H
#include <vector>
#include "Strategy.h"
class MarketConflict;
class MarketRequest;
class LocalMarketRequest;
class LocationEntity;
class FactionEntity;

/**
  *@author Alex Dribin
  */

class MarketStrategy : public Strategy  {
public: 
	MarketStrategy( const string & keyword, GameData * parent): Strategy(keyword, parent){};
	MarketStrategy( const MarketStrategy * prototype );
	~MarketStrategy(){}
   STATUS     initialize      ( Parser *parser);
   GameData * createInstanceOfSelf();
   void addRequest(MarketRequest * request);
   void process();
   void dailyPreProcess();
   void  save(ostream &out);
   void  report(FactionEntity * faction, ostream &out);
   void setLocation(LocationEntity * location);
protected:
   MarketConflict * marketConflict_;
   vector <LocalMarketRequest *> sellRequests_;
   vector <LocalMarketRequest *> buyRequests_;
   vector <LocalMarketRequest *> recruitRequests_;
   LocationEntity * location_;
};
typedef vector<LocalMarketRequest *>::iterator RequestIterator;

#endif
