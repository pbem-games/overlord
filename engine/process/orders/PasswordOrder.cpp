/***************************************************************************
                             PasswordOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "PasswordOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "FactionEntity.h"
#include "PhysicalEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
extern Reporter * changePasswordReporter;
//PasswordOrder  instantiatePasswordOrder;	
PasswordOrder * instantiatePasswordOrder = new PasswordOrder();

PasswordOrder::PasswordOrder(){
  keyword_ = "Password";
  registerOrder_();
  description = string("PASSWORD new-password \n") +
  "This order changes your faction password.  Said password is required in all\n" +
  "server commands (#GAME, #PRESS, #RUMOR).\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS PasswordOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
	if(!parseStringParameter(entity, parser,parameters))
        return IO_ERROR;
   if(entityIsPhysicalEntity(entity,NO_PARSING_REPORT))
            return OK;
   if(entityIsFaction(entity,NO_PARSING_REPORT))
            return OK;
    else    
  		return IO_ERROR;
}



ORDER_STATUS PasswordOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  string  newPassword = (parameters[0])->printName();
  FactionEntity * faction = dynamic_cast<FactionEntity *>(entity);
  if(faction)
    faction->setPassword(newPassword);
  PhysicalEntity * unit = dynamic_cast<PhysicalEntity *>(entity);
  if(unit)
    unit->getFaction()->setPassword(newPassword);
     
  entity->addReport(new UnaryPattern(changePasswordReporter,new StringData(newPassword))); 		
  return SUCCESS;
}
