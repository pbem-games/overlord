/***************************************************************************
                          BasicCollection.cpp  
              Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "BasicCollection.h"
#include <time.h>




BasicCollection::BasicCollection ()
{  

}



BasicCollection::~BasicCollection()
{

}





bool BasicCollection::isValidTag (const string &tag) const
{
 return false;
}


GameData* BasicCollection::operator [] (const string &tag)
{
 return 0 ;    
}






int  BasicCollection::getSize() const
{
  return 0;
  
}





int  BasicCollection::getIndex (const string &tag) const
{
  return 0;
}


void   BasicCollection::add  ( GameData * newRule)
{

}
void       BasicCollection::print ()
{
}
