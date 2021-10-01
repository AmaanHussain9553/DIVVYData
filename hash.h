/*hash.h*/

//
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Lab Week 11: hashing of dates and lottery numbers (SOLUTION)
// 

#include <string>

using namespace std;

bool isNumeric(string s);
int HashByID(int IDNumber, int N);
int HashByAbbrev(string abbreviation, int N);
int HashByTrip(string tripID, int N);
int HashByBike(string bikeID, int N);
