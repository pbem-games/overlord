/***************************************************************************
                          TeachOrder.cpp 
                             -------------------
    begin                : Thu Jul 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "TeachOrder.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "SkillLevelElementData.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
#include "PhysicalEntity.h"
#include "TeachingOffer.h"
extern Reporter * teachingReporter;
extern EntitiesCollection <UnitEntity>      units;

TeachOrder::TeachOrder()
{
   keyword_ = "teach";
  description = string("TEACH skill-tag unit-id [unit-id...] \n") +
  "Full-day, leader-only.  This order executes as soon as the specified unit is\n" +
  "present and issues the full-day order STUDY skill-tag.  Spends some time\n" +
  "teaching another unit.  Teaching is successful only when the teacher has a\n" +
  "greater level in the particular skill than the target unit.  Teaching enables\n" +
  "a unit to study a particular skill, get to an higher level and learn faster.\n";
    orderType_   = DAY_LONG_ORDER;
}

STATUS TeachOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
//   if(!entityIsUnit(entity))
//            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, skills, "skill tag", parameters))
            return IO_ERROR;

    while (parseOptionalGameDataParameter(entity, parser, units, "unit id", parameters))
          {}
            
  return OK;


}



ORDER_STATUS TeachOrder::process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId)
{
  PhysicalEntity * teacher = dynamic_cast<PhysicalEntity *>(entity);
  assert(teacher);

  SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]);

   if ( skill == 0)
    {
//	    ReportRecord * currentReport = new   ReportRecord(new UnaryPattern(cannotStudyReporter, unit->getRace()));
//		  unit->addReport( currentReport);
 		return INVALID;
    }
  vector <Entity * > students;
 PROCESSING_STATE  state = orderId->getProcessingState();

  switch(state)
   {
     case NORMAL_STATE:
        for (unsigned i=1; i< parameters.size(); ++i)
        {
          UnitEntity * newStudent = dynamic_cast< UnitEntity *>(parameters[i]);
          if(newStudent)
            {
              if (!teacher->mayInterract(newStudent)) // Not In the same place or can't see
	            continue;
              students.push_back(newStudent);
            }
        }
            teacher->addTeachingOffer(new TeachingOffer(teacher, skill, students));
            orderId->setProcessingState (SUSPEND);
            teacher->getLocation()->setTeacherCounter(true);
            return SUSPENDED;
        break;

     case SUSPEND:
     if(teacher->checkTeachingConfirmation())
     {
        // if this order is not the order that was processed last day
        // we may refrain from reporting
      if(teacher->getLastOrder() != orderId)
        {
          teacher->addReport(new UnaryPattern (teachingReporter,
                        new SkillLevelElementData(new SkillLevelElement
                        (skill,teacher->getSkillLevel(skill)))));
        }
      orderId->setProcessingState (RESUME);
            return SUSPENDED;
     }
     else
     {
      teacher->getLocation()->setTeacherCounter(false);
      orderId->setProcessingState (NORMAL_STATE);
      teacher->cancelTeachingOffer();
// 		  return INVALID;
 		  return FAILURE;
     }
     
        break;

     case RESUME:
      teacher->cancelTeachingOffer();
      teacher->getLocation()->setTeacherCounter(false);
      orderId->setProcessingState (NORMAL_STATE);
      return SUCCESS;
        break;

     default:
      cout << "Unknown state "<< state <<" in TEACH order\n";
   }
 		  return INVALID;


}



void TeachOrder::preProcess_(Entity * entity, vector <AbstractData *>  &parameters, Order * orderId)
{
  
}



void TeachOrder::doProcess_(Entity * entity, vector <AbstractData *>  &parameters, Order * orderId)
{
  
}
