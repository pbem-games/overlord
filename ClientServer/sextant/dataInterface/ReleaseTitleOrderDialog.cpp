/*
 * File:   ReleaseTitleOrderDialog.cpp
 * Author: alex
 * 
 * Created on May 1, 2012, 11:48 AM
 */
#include <sstream>
#include "ReleaseTitleOrderDialog.h"
#include "TitleRule.h"
#include "ViewManager.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
// YIELD title-tag location-id 
// CANCEL title-tag location-id 
OrderWindow * ReleaseTitleOrderDialog::show(TokenEntity * token, 
     OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    TitleRule * data=0;
    TitleElement * title;
    LocationEntity * location=0;
    UnitEntity * unit =  dynamic_cast<UnitEntity *>(token);
    if(unit==0)
    {
        return 0;
    }

    data = dynamic_cast<TitleRule *>(view->getActiveData());
    location = dynamic_cast<LocationEntity *>(view->getSelectedLocation());
            if((data)&&(location))
            {
              title = new TitleElement(data,location,unit);
            }


    
     OrderWindow * orderWindow = this->prepareWindow(token,order,view,"Release");
     if(orderWindow==0)
     {
         return 0;
     }
    

// for existing order determine parameters      
     if(!isNewOrder_ && params.size() >= 2)
     {
            data = dynamic_cast<TitleRule *>(params[0]);
            if(!data)
            {
                    cerr<<"ERROR: wrong  parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;
            }
            location = dynamic_cast<LocationEntity *>(params[1]);
            if(!location)
            {
                    cerr<<"ERROR: wrong  parameter ["<<(params[1])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;
            }
            if((data)&&(location))
            {
                if(title)
                {
                    delete title;
                }
              title = new TitleElement(data,location,0);
            }
     }
     if(title)
     {
        CB_ = new OvlElementComboBox(title);
     }
     else
            {
                 cerr<<"ERROR: wrong  title definition in order for "<<token <<endl;
            }
  // Add items to ComboBoxes and add widgets
     vector < TitleElement *> * titles =   unit->getTitlesAttribute()->getAll();
    for( vector < TitleElement *>::iterator iter =  titles->begin(); iter != titles->end(); ++iter)
     {
         if((*iter)->getTitle() != data || (*iter)->getTitleLocation() != location ) // Already added // Not added
         {
            CB_->addGameItem ((*iter));
         }
     }
      orderWindow->addWidget(CB_);
      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * ReleaseTitleOrderDialog::getOrderLine()
{
    stringstream s;
    string titleTag;
    string locationTag;


    AbstractData * data = CB_->getCurrentItem();
    if(data)
    {
      TitleElement *  title = dynamic_cast<TitleElement *>(data);
      if(title)
      {
        titleTag = title->getTitle()->getTag();
        locationTag = title->getTitleLocation()->getTag();
      }
    }
    s <<getKeyword()<<" "<<titleTag<<" "<<locationTag<<endl;
    return updateOrderLine(s.str());   
}

