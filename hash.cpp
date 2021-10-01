/*hash.cpp*/

//
// Amaan Hussain
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 

#include <iostream>
#include <string>
#include <cctype>		/*isdigit */
#include <regex>		/*regular expressions */

#include "hash.h"

using namespace std;

//
// isNumeric
//
// Returns true if the given string is numeric (all digits), false
// if not.  If the string is empty, false is returned since there 
// are no digits present.
//
bool
isNumeric (string s)
{
  //
  // A string is numeric if it contains 1 or more digits, so let's
  // use a regular expression to check that.
  //
  // we are using POSIX basic notation for regular expressions, see
  //   https://en.cppreference.com/w/cpp/regex/ecmascript
  // where [:d:] means digit, [[:d:]] means match a single digit.
  // The + means match the preceding sub-expression 1 or more times.
  //
  regex pattern ("[[:d:]]+", regex::ECMAScript);

  smatch matchResults;		// provides more details on the match

  if (regex_match (s, matchResults, pattern))
    return true;
  else
    return false;
}

//
// HashByID
// 
// There this takes in the ID which is already converted 
// into an int and is stored as an int in the hashmap 
//
int
HashByID (int IDNumber, int N)
{
  return IDNumber % N;
}

//
// Hash
//
// This is necessary so we can pass the existing gradescope tests,
// instead of having to create a new set of tests.  There's no
// other reason for it.
//
int
HashByID (int IDNumber)
{
  return HashByID (IDNumber, 10000);
}

//
// HashByAbbrev
// 
// This function takes in a string and does 
// multiplicative string hash in order to return
// a unique index for every possible string 
//
int
HashByAbbrev (string abbreviation, int N)
{

  unsigned long long hash = 5381;

for (char c:abbreviation)
    {

      hash = ((hash << 5) + hash) + c;
    }

  return hash % 10000;

}

//
// Hash
//
// This is necessary so we can pass the existing gradescope tests,
// instead of having to create a new set of tests.  There's no
// other reason for it.
//
int
HashByAbbrev (string abbreviation)
{
  return HashByAbbrev (abbreviation, 1);
}

//
// HashByTrio
// 
// This hash function erases the first two characters
// which is always "Tr" in the case of trip ID numbers
// and then mods by the size of the array
//
int
HashByTrip (string tripID, int N)
{

  tripID.erase (0, 2);

  return (stoi (tripID) % 2500000);

}

//
// Hash
//
// This is necessary so we can pass the existing gradescope tests,
// instead of having to create a new set of tests.  There's no
// other reason for it.
//
int
HashByTrip (string tripID)
{
  return HashByTrip (tripID, 2500000);
}

//
// HashByBike
// 
// This hash function gets rid of the first
// character which is always a "B" in the  
// case of BikeId's and then converts to
// int and mods by the size of the hashmap.
//
int
HashByBike (string bikeID, int N)
{


  bikeID.erase (0, 1);
  int index = stoi (bikeID);


  return index % 50000;

}

//
// Hash
//
// This is necessary so we can pass the existing gradescope tests,
// instead of having to create a new set of tests.  There's no
// other reason for it.
//
int
HashByBike (string bikeID)
{
  return HashByBike (bikeID, 50000);
}
