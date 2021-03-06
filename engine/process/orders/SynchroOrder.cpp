/***************************************************************************
                             SynchroOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "SynchroOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "SynchroOrderRequest.h"

SynchroOrder * instantiateSynchroOrder = new SynchroOrder();

SynchroOrder::SynchroOrder(){
  keyword_ = "Synchro";
  registerOrder_();
  description = string("SYNCHRO tokenEntity-id \n") +
  "Immediate, special condition.  This order executes only when the designated\n" +
  "tokenEntity issues a reciprocal SYNCHROSynchro tokenEntity-id.  Both tokenEntities will have the condition\n" +
  "execute at the same instant.  Only one SYNCHRO each day is possible.\n" +
  "\n" +
  "If both tokenEntities belong the same faction, SYNCHRO works anywhere within the game\n" +
  "world.  If the tokenEntities trying to synchronise belong to differing factions,\n" +
  "they must be able to interact to synchronise.\n";
  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS SynchroOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, gameFacade->units, "tokenEntity id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS SynchroOrder::process (Entity * entity, ParameterList &parameters)
{
  TokenEntity * tokenEntity = dynamic_cast<TokenEntity *>(entity);
  assert(tokenEntity);
  TokenEntity * synchroTarget =DOWNCAST_ENTITY<TokenEntity>(parameters[0]);
  if(synchroTarget == 0)

  	return INVALID;


  SynchroOrderRequest * tempSynchroRequest  = new SynchroOrderRequest(this,synchroTarget);
  // This is temporary request which is used for polymorphc access to OrderRequests
  // stored in tokenEntity.  It may be deleted if request already exists,
  // otherwise it is added to tokenEntity's OrderRequests

  if(tokenEntity->doneOrderSyncRequest(tempSynchroRequest))
  {
// Do work here

// delete request of this order
     tokenEntity->removeOrderSyncRequest(tempSynchroRequest) ;
     delete tempSynchroRequest;
      return SUCCESS;
  }

  BasicOrderSynchronizationRequest * synchroRequest  = tokenEntity->hasOrderSyncRequest(tempSynchroRequest);
  if(synchroRequest == 0)
  {
      synchroRequest = tempSynchroRequest;
      tokenEntity->addOrderSyncRequest(synchroRequest);
  }
  else
   {
     delete  tempSynchroRequest;
   }
  SynchroOrderRequest * tempTargetSynchroRequest  = new SynchroOrderRequest(this,tokenEntity);
  BasicOrderSynchronizationRequest * targetSynchroRequest = synchroTarget->hasOrderSyncRequest(tempTargetSynchroRequest);
  delete tempTargetSynchroRequest;
  if(targetSynchroRequest)
  {
      if(tokenEntity->getFaction() != synchroTarget->getFaction() )
      {
      if(!tokenEntity->mayInterractTokenEntity(synchroTarget)) // Units of the same faction
          return FAILURE;
      }
// Do work here

// delete request of this order and mark recipocal order for completeing
     synchroTarget->markDoneOrderSyncRequest(targetSynchroRequest);
     tokenEntity->removeOrderSyncRequest(synchroRequest) ;
      return SUCCESS;
  }

 else
      return FAILURE;
}
