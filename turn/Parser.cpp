/***************************************************************************
                          Parser.cpp  
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "Parser.h"
Parser::Parser ( const char *input )
{
 input_ = strcpy ( buffer_, input );
}



Parser::Parser ( const string &input )
{
 input_ = strcpy ( buffer_,input.c_str());
}



Parser::Parser()
{
}
//============================================================================
//
//   bool   matchKeyword (const char * keyword)
//
//  This function is intended for parsing of keyword-based text input.
//
// It is supposed that input is \n - terminated c-string starting from keyword. 
// (There may be also some spaces  before keyword). 
// matchKeyword case-insensitive looks for specified keyword at the beginning 
// of the string.
//
// If keyword found it shifts input string pointer to the right 
// so that resulting string it points to starts from the next  symbol after
//  keyword. Function in this case returns TRUE.
//
// If keyword was not found at the beginning of the string 
// function returns FALSE.
//
// This is modified  Vincent Archers's code.
// NOTE: it uses isspace and isalpha macro which are locale-dependent
//============================================================================
bool  Parser::matchKeyword (const char *keyword)
{
  char    *scan;
  scan = input_;

  while ( isspace (*scan) ) // skip extra spaces before keyword
    scan++;

  while (*keyword) {
    if (*keyword != *scan) {
      if ( !isalpha (*keyword) || !isalpha (*scan) )
		return false; // illegal characters in the input or in the keyword
	
      if ( (*keyword & 0x1F) != (*scan & 0x1F) )// after throwing away all 
     //non-alpha  symbols this works as case-nonsensitive comparison of symbols
	return false;
    }
    keyword++;
    scan++;
  }

  if ( isalnum (*scan) ) // the word in the input has the same beginning
                        //  as keyword, but longer.
    return false;
    
  input_ = scan;
  return true;
}


//============================================================================
//
//   bool   matchChar (const char)
//
//  This function is intended for parsing of keyword-based text input.
//
// It is supposed that input is c-string starting from specified character. 
// (There may be also some spaces  before it). 
// matchChar case-insensitive looks for specified character at the beginning 
// of the string.
//
// If character found it shifts input string pointer to the next 
// position after character. Function in this case returns TRUE.
//
// If character was not found at the beginning of the string 
// function returns FALSE.
//
// NOTE: it uses isspace  and isalpha macro which are locale-dependent
//============================================================================
bool  Parser::matchChar (const char symbol)
{
  char    *scan;
  scan = input_;

  while ( isspace (*scan) ) // skip extra spaces before symbol
    scan++;

  if(isalpha(symbol))
    {
      if ( (symbol & 0x1F) != (*scan & 0x1F) )
     //for alpha  symbols this works as case-nonsensitive comparison of symbols
	return false;
    }
  else
    {
      if (symbol != *scan)
	return false;
    }    
  input_ = scan + 1;
  return true;
}

//============================================================================
//
//      string  getParameter ()
//
// This function is intended for input parsing.
//
// It extracts first parameter from the input and interprets it as a single
//  word. Returns 0 if failed.
// Input string pointer shifted to the right 
// so that resulting string  it points to starts from the next  symbol after
// parameter.
// This is modified  Vincent Archers's code.
// NOTE: it uses isspace  macro which are locale-dependent
//============================================================================



string  Parser::getParameter ()
{


char    separatorSymbol;
char    *scan;


  while (isspace (*input_) ) // Skip spacing
    input_++;

  if (*input_ == '"') // Separator ?
    separatorSymbol = *input_++;
  else
    separatorSymbol = 0;

  scan = input_;
  while (*scan) { // Untill end of string
    if (separatorSymbol) {
      if (*scan == separatorSymbol) { // Closing separator was found.
	break;
      }
    } else 
      if (isspace (*scan) )  // Closing space was found.
	break;

    scan++;

  }
  string Parameter (input_, 0, scan - input_ );
  if (separatorSymbol)
    scan++;
  
    input_=scan;
    return Parameter;
}



//============================================================================
//
//      string  getWord ()
//
// This function is intended for input parsing.
//
// It extracts first word from the input
// Input string pointer shifted to the right 
// so that resulting string  it points to starts from the next  symbol after
//  word. Returns 0 if failed.
//
// NOTE: it uses isspace  macro which are locale-dependent
//============================================================================

string  Parser::getWord ()
{
char    *scan;

  while (isspace (*input_) ) // Skip spacing
    input_++;


  scan = input_;
  while (*scan) { // Untill end of string
    if (isspace (*scan) )  // Closing space was found.
      break;

    scan++;

  }
  string Word (input_, 0, scan - input_ );
  
  input_=scan;
 
    return Word;

}


//============================================================================
//
//      int  getInteger ()
//
// This function is intended for input parsing.
//
// It extracts first token from the input and interprets it as integer
// Input string pointer shifted to the right 
// so that resulting string  it points to starts from the next  symbol after
//  integer. Returns integer, prints error message if failed.
//
// NOTE: it uses isspace and isdigit macro which are locale-dependent
//============================================================================

int  Parser::getInteger ()
{
int i; 
char buffer[INTEGER_LENGTH]; // Integers should not be bigger than 65535 anyway



  while (isspace (*input_) ) // Skip spacing
    input_++;

  for (i=0; i<INTEGER_LENGTH; i++)
    {
      if (!isdigit (*input_) )  // Non-digit was found
	break;
      if (!*input_)  // End of string
	break; 
      buffer[i] = *input_;
      *input_++;
    }
  assert ( (i >=  1) && (i <  INTEGER_LENGTH ) ); // At least one 
    // but less than 6 digits were found.

  buffer[i] = '\0';// Terminate string.
  return atoi (buffer);

}

//============================================================================
//
//      bool  matchInteger ()
//
// This function is intended for input parsing.
//
// It extracts first token from the input and checks it for being integer
// in the case of success returns TRUE, else - FALSE 
// Input string pointer shifted in the position of the first isdigit symbol. 
//
// NOTE: it uses isspace and isdigit macro which are locale-dependent
//============================================================================

int  Parser::matchInteger ()
{
int i; 
char    *scan;
char buffer[INTEGER_LENGTH]; // Integers should not be bigger than 65535 anyway



  while (isspace (*input_) ) // Skip spacing
    input_++;
  scan = input_;
  for (i=0; ; i++)
    {
      if (!isdigit (*scan) )  // Non-digit was found
	break;
      if (!*scan)  // End of string
	break; 
      buffer[i] = *scan;
      *scan++;
    }
  if ( (i >=  1) && (i <  INTEGER_LENGTH ) ) // At least one 
    // but less than 6 digits were found.
    return true;
  else
    return false;


}

//============================================================================
//
//      string  getText ()
//
// This function is intended for input parsing.
//
// It returns the rest of the line as a string

//============================================================================

string  Parser::getText ()
{
  return string (input_);
}



Parser::~Parser ()
{

}
void Parser::getLine()
{

}

bool Parser::eof()
{
 return 0;
}

void Parser::setPosition ( int position )
{

}
int Parser::getPosition()
{
  return 0;
}
