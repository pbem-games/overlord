/***************************************************************************
                          BasicCollection.h

    Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef BASIC_COLLECTION_H
#define BASIC_COLLECTION_H
#include <string>
//#include "SystemTypes.h"
#include "OverlordTypes.h"
using namespace std;
class  DataStorageHandler;
class  GameData;
class  NewEntityPlaceholder;

/** Basic Collection of Polymorphic Persistent objects */
class BasicCollection {
public:
	  BasicCollection(DataStorageHandler * handler){ bindHandler(handler);}
	  virtual ~BasicCollection();
	virtual GameData* findByTag        (const string &tag, bool errorReportEnabled = true);
	virtual GameData* findByIndex      (const long intindex, bool errorReportEnabled = true);
	virtual NewEntityPlaceholder * findPlaceholder(const string &tag);
  virtual void       add  ( GameData * /*const*/ newRule);
  virtual bool      isValidTag (const string &tag)  ;//const;
  virtual bool      checkDataType    (const string &tag);
	virtual long       size()                       const;  
  virtual long       getIndex    (const string &tag);
  virtual void    redimention (long intsize);
          void    bindHandler(DataStorageHandler * handler);
  inline  DataStorageHandler * getStorageHandler() const { return handler_;}
          void setCollectionKeyword(const string &keyword) {collectionKeyword_ =keyword; }// not needed
  inline  STATUS getStatus() const {return status;}
  inline  string getCollectionKeyword() const {return collectionKeyword_;}
  inline  string printName() const {return collectionKeyword_;}
  virtual void addRIPindex(long intindex){}
  virtual long int getRIPsize() const{return 0;}
  virtual long int getRIPbyIndex(long int i) const {return 0;}
  
 protected:
		string collectionKeyword_;
    DataStorageHandler * handler_;
    STATUS status;
private:


};

#endif

