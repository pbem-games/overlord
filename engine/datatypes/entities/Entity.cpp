/***************************************************************************
                          Entity.cpp  -  description
                             -------------------
    begin                : Sun Nov 17 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <algorithm>
#include "Entity.h"
#include "ReportRecord.h"
#include "ReportElement.h"
#include "InventoryElement.h"
#include "TeachingOffer.h"

extern bool testMode;

Entity::~Entity()
{

}


//  Processes all possible ( at this phase) orders for Entity.
// Each order processed only once
bool  Entity::process(ProcessingMode * processingMode)

{
  bool orderWasExecuted = false;
  vector<Order *>::iterator currentIterator ;
  ORDER_STATUS result;
#ifdef TEST_MODE
   if(testMode) 	cout<< "Processing orders for Entity " << printName() <<endl;
#endif

   for( currentIterator = orders_.begin(); currentIterator != orders_.end();)
     {
			if( (*currentIterator)->ifConditionLevel > 0)
					{
						currentIterator++;
						continue;
					}
      result = (*currentIterator) ->process(processingMode,  this,cout);
      if (result == SUSPENDED) // second pass needed
        return true;

      orderWasExecuted = processOrderResults(result,currentIterator);
     }// End of orders loop
 if(!orderWasExecuted)
			; // process default order for this Entity(processingMode,this,cout)
   return orderWasExecuted;
}

bool Entity::updateOrderResults(ORDER_STATUS result, Order * OrderId)
{
 vector<Order *>::iterator  currentIterator = find(orders_.begin(), orders_.end(), OrderId);
 return processOrderResults(result,currentIterator);

}

bool Entity::processOrderResults(ORDER_STATUS result, vector<Order *>::iterator & currentIterator)
{
  assert (result != SUSPENDED);

  bool orderWasExecuted = false;   
  if((result == SUCCESS) || (result == IN_PROGRESS))
  {
          if ((*currentIterator)->isFullDayOrder())
               {
//               cout << "Full-day order "; (*currentIterator)->save(cout); cout <<endl;
                  lastOrder_ = (*currentIterator);
                  setFullDayOrderFlag();
               }
  }

  if(result != FAILURE )
      	{
					orderWasExecuted = true;
				}
        
  switch (result)
        {
					case SUCCESS:
	  				{
#ifdef TEST_MODE
   if(testMode) 	    cout << "==== Result of order processing is Success" << endl;
#endif
	    				postProcess(result, currentIterator);

	    				if ((*currentIterator) -> repetitionCounter() > 1)
	      				{
         					(*currentIterator)->decrementRepetitionCounter()  ;
									currentIterator++;
				 					break;
	      				}

	    				if ((*currentIterator)->isPermanent())
	      				{
									currentIterator++;
				 					break;
	      				}

	    				else
	      				{
				    			(*currentIterator) -> ~Order();
		    					  currentIterator = orders_.erase(currentIterator);
		  					}
							break;
	  				}
			case FAILURE:
	  				{
#ifdef TEST_MODE
   if(testMode) 	    cout << "==== Result of order processing is Failure" << endl;
#endif
							currentIterator++;
	    				break;
	  				}
			case INVALID:
	  				{
#ifdef TEST_MODE
   if(testMode) 	    cout << "==== Result of order processing is Invalid" << endl;
#endif
	    				postProcess(result, currentIterator);

				    	(*currentIterator) -> ~Order();
		    			currentIterator = orders_.erase(currentIterator);
	    				break;
	  				}//end of INVALID case
			case IN_PROGRESS:
          {
#ifdef TEST_MODE
   if(testMode) 	    cout << "==== Order is in progress" << endl;
#endif
	    				break;
            }
			case SUSPENDED:
	    				break;
	  //default:
				}// End of result switch
  return orderWasExecuted;      
}



void 	    
Entity::postProcess(ORDER_STATUS result, vector<Order *>::iterator  iter)

{
  vector<Order *>::iterator    currentIterator = iter;
  currentIterator++;	
  for ( ; currentIterator != orders_.end(); )
    {
			if( !((*currentIterator)->whileCondition()) && !((*currentIterator)->ifConditionLevel > 0))
			return;		
#ifdef TEST_MODE
   if(testMode)
			{
				cout << "====+++ Post-processing " ;      (*currentIterator)->save(cout);
			}
#endif
      switch(result)
			{
				case SUCCESS:
	  			{
	    			if ((*currentIterator)->ifConditionLevel > 0)
	      			{
								(*currentIterator)->ifConditionLevel--;
							}
		// Checking next conditional modifier
		// if it is "+" Order will be deleted in the next "if" (combination "-+" is illegal (?))
		// otherwise put "else"
	
	    			if ((*currentIterator)->whileCondition()) // Delete order
	      			{
#ifdef TEST_MODE
   if(testMode) 		cout << "====+++ Order deleted (condition failed)"<<endl;
#endif
				    	(*currentIterator) -> ~Order();
		    			currentIterator = orders_.erase(currentIterator);
	      			}
	      		else
	      			{
		    				currentIterator++;
		    			}
	    		break;
	  		}
			case INVALID:
	  		{
	    		if ((*currentIterator)->whileCondition()) // +
	      		{
					 		(*currentIterator)->setWhileCondition(false);
								// Checking next conditional modifier
								// if any exists delete order
							if ((*currentIterator)->ifConditionLevel > 0) // Delete order (and Node)
		  					{
#ifdef TEST_MODE
   if(testMode) 		    cout << "====+++ Order deleted (impossible conditions)"<<endl;
#endif
		    					(*currentIterator)  ->  ~Order();
		    						currentIterator = orders_.erase(currentIterator);
		  					}
							else
		    						currentIterator++;						
	    				break;
	      		}
	
	    		if ((*currentIterator)->ifConditionLevel > 0) // Delete order
	      		{
#ifdef TEST_MODE
   if(testMode) 		cout << "====+++ Order deleted (condition failed)"<<endl;
#endif
				    	(*currentIterator) -> ~Order();
		    			currentIterator = orders_.erase(currentIterator);
	      		}
	    		break;
	  		}
				case FAILURE:
				case IN_PROGRESS:
				case SUSPENDED:
	    		break;
		} // switch
     
   }// end of for cycle
}


GameData * Entity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<Entity> (this);
}

STATUS
Entity::initialize        ( Parser *parser )
{

	
      if (parser->matchKeyword ("NAME") )
        {
	  setName(parser->getText());
	  return OK;
	}
      if (parser->matchKeyword("DESCRIPTION"))
	{
	  setDescription(parser->getText());
	  return OK;
	}
       if (parser->matchKeyword("ORDER"))
 	{
	  orders_.push_back(new Order(parser->getText(),this));
 	}

	  return OK;

}
void
Entity::save(ostream &out)
{
  out << keyword_ << " " <<tag_ << endl;
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
  out << endl;
  vector<Order *>::iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->save(out);
    }
}
ostream &operator << ( ostream &out, Entity * entity)
{
  out << entity->getTag() << " ";
	return out;
}


void  Entity::loadOrders()

{
//
//  vector<Order *>::iterator iter;
//  for ( iter = orders_.begin(); iter !=  orders_.end(); iter++)
//    {
//           (*iter)->load();
//    }


}

void Entity::print() // For debugging
{
    cout  << printName()<< endl;

  vector<Order *>::iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->save(cout);
    }

}





void Entity::addOrder(Order * newOrder)
{
	orders_.push_back(newOrder);
}


/** No descriptions */
void Entity::clearOrders()
{
  vector<Order *>::iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           delete (*iter);
    }
  orders_.clear();
}
STATUS Entity::prepareData()
{
//	STATUS status = OK;
	preprocessData();    // Re-establish internal references, there they  were not saved.
   if(IO_ERROR == dataConsistencyCheck())    // Check consistency of data
      return IO_ERROR;
//   prepareOrders();
		return OK;
}



void Entity::preprocessData()
{
}



void Entity::dailyPreProcess()
{
}

///** initialize secondary navigation system in OrderNode collection */
//void Entity::prepareOrders()
//{
//  OrderModifier * currentModifier;
//  OrderNode * lastValidOrderNode =0;
//  vector<OrderNode *>::iterator iter;
//#ifdef TEST_MODE
//      //if(testMode) cout << "Pre-processing orders for entity " << getTag() <<endl;
//#endif
//
//  for ( iter = orders_.begin(); iter != orders_.end(); )
//    {
//
//#ifdef TEST_MODE
//     //if(testMode) (*iter)->getOrder()->save(cout);
//#endif
////    cout <<"==== "; (*iter)->getOrder()->save(cout);
//
//      currentModifier =  (*iter)->getOrder()->getTopConditionModifier();
//      if (currentModifier == 0)
//	{
////    cout <<"====  No conditional modifiers" <<endl;
//	  (*iter) -> setActive(true);
//	  if (lastValidOrderNode !=0)
//	    {
//	      lastValidOrderNode->setNext( (*iter) );
//	      (*iter) -> setLast(lastValidOrderNode);
//	    }
//	
//	  lastValidOrderNode = (*iter);
//		iter++;
//	  continue;
//	}
//      else		
//	{
//	  if (lastValidOrderNode ==0)
//	    {// all conditional orders at the beginning are mistake and should be deleted
//	      (*iter)  ->  ~OrderNode();
//	      iter = orders_.erase(iter);
//	    }
//	  else
//	    {
//	      (*iter) -> setActive(false);
//			iter++;
//	    }	
//	}
//
//    }
//}
/** Adds public report */
void Entity::addReport(ReportRecord * report)
{
	publicReports_.push_back(report);
// cout << "Added report "; report->print(cout);
}
void Entity::addReport(ReportPattern * report,Order *  orderId, BasicCondition * observationCriteria)
{
publicReports_.push_back(new  ReportRecord(report, orderId, observationCriteria));
}
/** No descriptions */
void Entity::dailyUpdate()
{
}
#include "entities/UnitEntity.h"
 /** No descriptions */
void Entity::extractReport(UnitEntity * unit, vector < ReportElement * > & extractedReports)
{
//	cout << "Extracting Reports for [" <<tag_ <<"] " << unit->printTag()<< endl;
   vector<ReportRecord *>::iterator iter;
  for ( iter = publicReports_.begin(); iter != publicReports_.end(); iter++)
    {
       if( (*iter)->observableBy(unit))
			{
//             cout << "Report extracting by "<< unit->printName()<<" =} "/*<< <<endl*/;(*iter)->reportMessage->print(cout);
            	extractedReports.push_back
							(new ReportElement((*iter)->reportMessage,this));
			}			
	}
}
/** Returns reference to Entity, which contains reports about this */
Entity * Entity::getReportDestination(){
return 0;
}
/** prints  report for Entity (stats, posessions, private events) */
void Entity::report(FactionEntity * faction, ostream &out){
 out  << printName()<<endl;
// Stats
// Posessions
 reportEvents(out);
}
/** prints list of events related to this Entity */
void Entity::reportEvents(ostream &out)
{
   vector<ReportElement *>::iterator iter;
  for ( iter = collectedReports_.begin(); iter != collectedReports_.end(); iter++)
    {
		out << "    "; (*iter)->print(out);
	}
}
/** Transforms public reports into collected reports and cleans all unused public reports. */
void Entity::finalizeReports()
{
//	for_each(publicReports_.begin(),publicReports_.end(),  )
//cout << "Finalizing report for " <<printTag()<<endl;
// It is possible that exist two or more duplicates of the same report
// (results of multiple attempts of the execution of the same order)
// the earlier one should be deleted

   vector<ReportRecord *>::iterator iter1;
   vector<ReportRecord *>::iterator iter2;
		bool duplicate;

  for ( iter1 = publicReports_.begin(); iter1 != publicReports_.end(); )
		{
       duplicate = false;
       if((*iter1)->orderId == 0)
            break; // This is non-order generated report
  		for ( iter2 = iter1 + 1; iter2 != publicReports_.end(); iter2++)
					{
					if( (*iter2)->orderId == (*iter1)->orderId)
							{
							  duplicate = true;
								break;
							}
		    	}
			if (duplicate)
					{
					delete (*iter1);
					publicReports_.erase(iter1);	
					}
			else
					iter1++;
		}

  for ( iter1 = publicReports_.begin(); iter1 != publicReports_.end(); iter1++)
		{
					collectedReports_.push_back
							(new ReportElement((*iter1)->reportMessage,this));
		}
}
/** cleans all unused public reports. */
void Entity::cleanReports()
{
   vector<ReportRecord *>::iterator iter;
  for ( iter = publicReports_.begin(); iter != publicReports_.end(); iter++)
		{
					delete (*iter);
		}
					publicReports_.clear();
}


bool      Entity::defaultAction()
{
  return false;
}


bool Entity::mayInterract(UnitEntity * unit)
{
        return false;
}


void Entity::clearTeachingOffers()
{
  teachingOffers_.clear();
//  cout << *this << " clears all teachin offers"<<endl;
}


void Entity::addTeachingOffer(TeachingOffer * offer)
{
 teachingOffers_.push_back(offer);
// cout <<*offer << " added to "<< *this<<endl;
}
int Entity::getSkillLevel(SkillRule  * const skill)
{
//	SkillIterator iter;
//	for (iter = skills_.begin(); iter != skills_.end(); iter++)
//		{
//			if (skill == (*iter).getSkill())
//				{
//          return skill->getLevel((*iter).getExpPoints() );
//          }
//		}
  return 0;
}
TeachingOffer * Entity::findTeachingOffer(SkillRule  * skill, int level)
{
  vector <TeachingOffer  *>::iterator iter;
  for(iter = teachingOffers_.begin(); iter != teachingOffers_.end(); ++iter)
  {

    if((*iter)->getSkill() == skill && (*iter)->getLevel() >= level)
        return (*iter);

    }
 return 0;
}
bool Entity::checkTeachingConfirmation()
{
  vector <TeachingOffer  *>::iterator iter;
  for(iter = teachingOffers_.begin(); iter != teachingOffers_.end(); ++iter)
  {
    if((*iter)->getTeacher() == this)
        return (*iter)->isConfirmed();
    }
  cout << "ERROR."<< printName() <<" Can't find his own teachingOffers\n";
  return false;
}
/** How many seats occupies this entity in the class. Number of entities that can be tought by one teacher determined by this value. */

int Entity::getLearningCapacity()
{
  return 10000; // very big. can't study
}

bool Entity::teacherRequired(SkillRule * skill) 
{
  return false;
}



bool Entity::addKnowledge(Rule * info)
{
  return false;
}



bool Entity::addSkillKnowledge(SkillRule * knowledge, int level)
{
  return false;
}
