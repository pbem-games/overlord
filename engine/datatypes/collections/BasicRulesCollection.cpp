/***************************************************************************
                          BasicRulesCollection.cpp
              Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <time.h>
#include "BasicRulesCollection.h"
#include "rules/Rule.h"
#include "DataStorageHandler.h"
BasicRulesCollection::~BasicRulesCollection()
{
  // delete handler_; // <- check
}


GameData* BasicRulesCollection::findByTag (const string &tag)
{
RulesIterator iter;
 for (iter = begin(); iter != end(); iter++)
   {
      if ((*iter) ->getTag() == tag)
       return  (*iter);
   }

 cerr << "Error: Tag (" << tag  << ") was not found in "<<collectionKeyword_<<"-s collection\n";
 return 0 ;
}
GameData* BasicRulesCollection::findByIndex (const long int index)
{
	if ( index <= size())
         return data_[index];
 cerr << "Error: Array index (" << index << ") is out of array dimensions!\n";
 return 0 ;
}

void   BasicRulesCollection::add  ( GameData * /*const*/ newRule)
{
  data_.push_back ( dynamic_cast< Rule*> (newRule) );
//  vector <Rule *>::iterator iter = data_.end() - 1;
//  if (*iter) cout <<" inside " << (*iter)-> printName()  <<endl;
}




bool BasicRulesCollection::isValidTag (const string &tag) //const
{
RulesIterator iter;
 for (iter = begin(); iter != end(); iter++)
   {
      if ((*iter) ->getTag() == tag)
       return true;

   }
 return false;
}



long int  BasicRulesCollection::getIndex (const string &tag)
{
RulesIterator iter;
 long int i=0;

 for (iter = begin(); iter != end(); iter++)
   {
      if ((*iter) ->getTag() == tag)
       return i;
      i++;
   }
 return i-1;

}


