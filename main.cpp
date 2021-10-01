/*main.cpp*/

//
// Amaan Hussain
// U. of Illinois, Chicago
// CS 251: Spring 2020
//  

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include "util.h"
#include <vector>
#include <algorithm>

#include "hash.h"
#include "hashmap.h"

//
// This struct stores all the information
// about the stations
//
struct StationInfo
{

  int ID;
  string Abbrev;
  string Fullname;
  double Latitude;
  double Longitude;
  int Capacity;
  string OnlineDate;
    
  StationInfo(){
      ID = 0;
      Abbrev = "";
      Fullname = "";
      Latitude = 0.0;
      Longitude = 0.0;
      Capacity = 0;
      OnlineDate = "";
  }  

};

//
// This struct stores all the information
// about the trips
//
struct TripInfo
{
  string TripID;
  string StartTime;
  string StopTime;
  string BikeID;
  int SecDuration;
  int StationFrom;
  int StationTo;
  string Gender;
  string BirthYear;
    
  TripInfo(){
      TripID = "";
      StartTime = "";
      StopTime = "";
      BikeID = "";
      SecDuration = 0;
      StationFrom = 0;
      StationTo = 0;
      Gender = "";
      BirthYear = "";
  }
};

//
// string2int
// 
// Converts a string to an integer, unless string is empty, in
// which case 0 is returned.
// 
int
string2int (string s)
{
  if (s == "")
    return 0;
  else
    return stoi (s);
}

//
// inputStationData
// 
// Given a filename it stores all the data in relation to the stations
// into two different hashmap. Once map where the key is the ID and the
// other where the key is an abbreviation of the station
// 
int
inputStationData (string filename, hashmap < int, StationInfo > &stationByID,
		  hashmap < string, StationInfo > &stationByAbbrev)
{
  ifstream infile (filename);

  cout << "Reading " << filename << endl;

  if (!infile.good ())
    {
      cout << "**Error: unable to open '" << filename << "'..." << endl;
      exit (0);
    }

  // file is open, start processing:
  string line;

  getline (infile, line);	// input and discard first row --- header row

  int count = 0;

  while (getline (infile, line))
    {
      stringstream s (line);

      string id, abbrev, fullname, latitude, longitude, capacity, onlinedate;

      //
      // format: id,abbrev,fullname,latitude,longitude,capacity,onlinedate
      // 
      // 
      getline (s, id, ',');
      getline (s, abbrev, ',');
      getline (s, fullname, ',');
      getline (s, latitude, ',');
      getline (s, longitude, ',');
      getline (s, capacity, ',');
      getline (s, onlinedate, ' ');

      //
      // store into hash map
      // 
      StationInfo ld;

      ld.ID = stoi (id);
      ld.Abbrev = abbrev;
      ld.Fullname = fullname;
      ld.Latitude = stod (latitude);
      ld.Longitude = stod (longitude);
      ld.Capacity = stoi (capacity);
      ld.OnlineDate = onlinedate;

      //insert into both hashmaps
      stationByID.insert ((stoi (id)), ld, HashByID);
      stationByAbbrev.insert (abbrev, ld, HashByAbbrev);

      count++;
    }
  return count;
}

//
// inputTripData
// 
// Given a filename it stores all the data in relation to the trip.
// This functions all stores all the bikeIds into a hashmap for bikesByID
// and counts at the number of times a bike was used in trips.csv
// 
int
inputTripData (string filename, hashmap < string, TripInfo > &tripsByID,
	       hashmap < string, int >&bikesByID, int &numberOfBikes)
{

  ifstream infile (filename);

  cout << "Reading " << filename << endl;

  if (!infile.good ())
    {
      cout << "**Error: unable to open '" << filename << "'..." << endl;
      exit (0);
    }

  // file is open, start processing:
  string line;

  getline (infile, line);	// input and discard first row --- header row

  int count = 0;

  while (getline (infile, line))
    {
      stringstream s (line);

      string tripid, starttime, stoptime, bikeid, secduration,
	stationfrom, stationto, gender, birthyear;

      //
      // format: tripID,starttime,stoptime,bikeid,secduration,stationfrom,stationto,gender,birthyear 
      // 
      // 
      getline (s, tripid, ',');	// first value => region
      getline (s, starttime, ',');	// second value => country
      getline (s, stoptime, ',');
      getline (s, bikeid, ',');
      getline (s, secduration, ',');
      getline (s, stationfrom, ',');
      getline (s, stationto, ',');
      getline (s, gender, ',');
      getline (s, birthyear, ' ');

      //
      // store into hash table
      // 
      TripInfo ld;

      ld.TripID = tripid;
      ld.StartTime = starttime;
      ld.StopTime = stoptime;
      ld.BikeID = bikeid;
      ld.SecDuration = stoi (secduration);
      ld.StationFrom = stoi (stationfrom);
      ld.StationTo = stoi (stationto);
      ld.Gender = gender;
      ld.BirthYear = birthyear;

      //insert into the tripsbyID hash function
      tripsByID.insert (tripid, ld, HashByTrip);

      //process to add bikeId data to bikeID hashmap
      int usagecount = 0;
      bool bikeCount = bikesByID.search (bikeid, usagecount, HashByBike);

      if (!bikeCount)
	{

	  usagecount = 1;
	  bikesByID.insert (bikeid, usagecount, HashByBike);
	  numberOfBikes++;
	}
      else
	{

	  usagecount++;
	  bikesByID.insert (bikeid, usagecount, HashByBike);

	}

      count++;
    }

  return count;

}

//
// This function converts seconds into minutes and hours and display
//
void
displayDuration (int seconds)
{

  int minutesConvert = (seconds / 60);
  int secondsConvert = (seconds % 60);
  cout << minutesConvert << " minutes, " << secondsConvert << " seconds";
}

//
// This function executes the nearby command of the program 
// This project reads in the entire userInput and accordingly finds all the 
// stations that are in close proximity
//
void
nearbyExecution (hashmap < int, StationInfo > stationByID, string & userInput)
{

  //reads in latitude and longitude and distance
  stringstream s (userInput);
  string latitude1;
  string longitude1;
  string userDistance;
  string command;

  getline (s, command, ' ');
  getline (s, latitude1, ' ');
  getline (s, longitude1, ' ');
  getline (s, userDistance, ' ');


  multimap < double, int >nearbyStations;
  StationInfo data;

  //stores all keys in vector from stationByID
  vector < int >stationIDKeys = stationByID.getVectorWithKeys (stationIDKeys);

  //iterates through vector to find the distance of each stations lat and long
  for (unsigned int i = 0; i < stationIDKeys.size (); i++)
    {
      if (stationByID.search (stationIDKeys[i], data, HashByID))
	{

	  double longitude2 = data.Longitude;
	  double latitude2 = data.Latitude;
	  double stationDistance =
	    distBetween2Points (stod (latitude1), stod (longitude1),
				latitude2, longitude2);

	  //checks distance and accordingly adds to map
	  if (stationDistance <= stod (userDistance))
	    {
	      nearbyStations.insert (pair < double,
				     int >(stationDistance,
					   stationIDKeys[i]));
	    }

	}
    }

  //print message
  cout << "Stations within " << userDistance << " miles of (" << latitude1 <<
    ", " << longitude1 << ") " << endl;
  if (nearbyStations.size () == 0)
    {
      cout << " none found" << endl;
    }
  else
    {
    for (auto pair:nearbyStations)
	{
	  cout << " station " << pair.second << ": " << pair.
	    first << " miles" << endl;
	}
    }
  cout << endl;

}

//
// This functions sorts through any file and arranges it in
// ascending order using bubble sort
//
void
sortNumeric (vector < int >&file)
{
  for (unsigned int j = 0; j < file.size () - 1; j++)
    {
      for (unsigned int i = 0; i < file.size () - 1; i++)
	{
	  if (file[i] > file[i + 1])
	    {
	      //for alphabetical order: a < b
	      int swap;
	      //creating a temp variable to swap later
	      swap = file[i + 1];
	      file[i + 1] = file[i];
	      file[i] = swap;
	    }
	}
    }
}

//
// This function counts the trip and returns the number of repetitions accordingly
//
void
findTripCount (vector < int >FromNearby, vector < int >ToNearby,
	       hashmap < string, TripInfo > &tripByID, int &tripCount)
{
  TripInfo data;
  vector < string > tripIDKeys;

  tripIDKeys = tripByID.getVectorWithKeys (tripIDKeys);

  for (unsigned int j = 0; j < tripIDKeys.size (); j++)
    {				// now loop though all the trips and see if any 
      tripByID.search (tripIDKeys[j], data, HashByTrip);	// have the same from and to stations as in the set


      if (count (FromNearby.begin (), FromNearby.end (), data.StationFrom)
	  && count (ToNearby.begin (), ToNearby.end (), data.StationTo))
	{
	  tripCount++;
	}
    }
}

//
// This function does the execution for the similar command by taking in the tripNumber and accordingly
// recording the station from and to of the tripNumber and then obtaining the latitude and longitude of 
// the to and from stations and then first using the lat and longitude of the from stations to do the 
// same thing as we didn in nearby treating the latitude and longitde of the from station as userInput.
// And then doing the same thing with the to stations.
//
void
similarExecution (hashmap < string, TripInfo > tripsByID, hashmap < int,
		  StationInfo > stationByID, string & userInput)
{

  //reading in the tripID and distance
  stringstream s (userInput);

  string tripID;
  string command;
  string distance;

  getline (s, command, ' ');
  getline (s, tripID, ' ');
  getline (s, distance, ' ');


  cout << "Trips that follow a similar path (+/-" << distance << " miles) as "
    << tripID << endl;

  TripInfo data;
  StationInfo dataFrom;
  StationInfo dataTo;

  // checking if the tripID exists
  if (!tripsByID.search (tripID, data, HashByTrip))
    {
      cout << " no such trip" << endl << endl;
      return;
    }

  // Ontaining the to and from stations ID
  tripsByID.search (tripID, data, HashByTrip);
  int statFrom = data.StationFrom;	//ID for start station
  int statTo = data.StationTo;	// 

  //Obtaining the latitude and longitude of the from stations
  stationByID.search (statFrom, dataFrom, HashByID);
  double startLat = dataFrom.Latitude;
  double startLon = dataFrom.Longitude;

  // Obtaininig the latitude and longitude of the to stations
  stationByID.search (statTo, dataTo, HashByID);
  double endLat = dataTo.Latitude;
  double endLon = dataTo.Longitude;


  //First to do this vector for from paths
  // Repeating the provess from the nearby functions
  StationInfo dataF;
  multimap < double, int >fromPath;
  vector < int >stationIDKeys = stationByID.getVectorWithKeys (stationIDKeys);

  for (unsigned int i = 0; i < stationIDKeys.size (); i++)
    {
      if (stationByID.search (stationIDKeys[i], dataF, HashByID))
	{

	  double longitude2 = dataF.Longitude;
	  double latitude2 = dataF.Latitude;
	  double stationDistance =
	    distBetween2Points (startLat, startLon, latitude2, longitude2);

	  if (stationDistance <= stod (distance))
	    {
	      fromPath.insert (pair < double,
			       int >(stationDistance, stationIDKeys[i]));
	    }

	}
    }
  vector < int >fromValues;
  cout << " nearby starting points: ";
for (auto pair:fromPath)
    {
      fromValues.push_back (pair.second);
    }
  sortNumeric (fromValues);
  //Printing out all the stations close to the from stations
  for (unsigned int i = 0; i < fromValues.size (); i++)
    {
      cout << fromValues[i] << " ";
    }


  // Lastly to do this vector for to paths
  // Repeating the provess from the nearby functions
  StationInfo dataT;
  multimap < double, int >ToPath;

  for (unsigned int i = 0; i < stationIDKeys.size (); i++)
    {
      if (stationByID.search (stationIDKeys[i], dataT, HashByID))
	{

	  double longitude2 = dataT.Longitude;
	  double latitude2 = dataT.Latitude;
	  double stationDistance =
	    distBetween2Points (endLat, endLon, latitude2, longitude2);

	  if (stationDistance <= stod (distance))
	    {
	      ToPath.insert (pair < double,
			     int >(stationDistance, stationIDKeys[i]));
	    }

	}
    }
  vector < int >ToValues;
  cout << endl << " nearby ending points: ";
for (auto pair:ToPath)
    {
      ToValues.push_back (pair.second);
    }
  sortNumeric (ToValues);
  //Printing out all the stations close to the to stations 
  for (unsigned int i = 0; i < ToValues.size (); i++)
    {
      cout << ToValues[i] << " ";
    }

  // calculating and printing out the tripCount
  int tripCount = 0;
  findTripCount (fromValues, ToValues, tripsByID, tripCount);
  cout << endl << " trip count: " << tripCount;
  cout << endl << endl;

}

//
// This function prints out the help commands
// displaying all the commands that the user can input
//
void
help ()
{
  cout << "Available commands: " << endl
    << " Enter a station id (e.g. 341)" << endl
    << " Enter a station abbreviation (e.g. Adler)" << endl
    << " Enter a trip id (e.g. Tr10426561)" << endl
    << " Enter a bike id (e.g. B5218)" << endl
    << " Nearby stations (e.g. 41.86 -87.62 0.5)" << endl
    << " Similar trips (e.g. similar Tr10424639 0.3)" << endl << endl;
}

//
// This function displays all the station Data taking a struct 
// as a parameter. Helper function for lookup in stationBy Abbrev
// and stationByID
//
void
printStationInfo (StationInfo userNeed, string userInput)
{
  cout << "Station: " << endl
    << " ID: " << userNeed.ID << endl
    << " Abbrev: " << userNeed.Abbrev << endl
    << " Fullname: " << userNeed.Fullname << endl
    << " Location: (" << userNeed.Latitude << ", " << userNeed.
    Longitude << ")" << endl << " Capacity: " << userNeed.
    Capacity << endl << " Online date: " << userNeed.
    OnlineDate << endl << endl;
}

//
// This function prints out all the data given for a trip based on tripID 
// and is a helper function to lookup up everything based on tripID
//
void
printTripInfo (hashmap < string, TripInfo > tripsByID, TripInfo userTripEnq,
	       hashmap < int, StationInfo > stationByID, string userInput,
	       StationInfo userNeed)
{
  if (tripsByID.search (userInput, userTripEnq, HashByTrip))
    {

      //Finiding station to and from's name to display 
      // searching through stationsByID hashmap to get Abbrev
      stationByID.search (userTripEnq.StationFrom, userNeed, HashByID);
      string stationFrom = userNeed.Abbrev;
      stationByID.search (userTripEnq.StationTo, userNeed, HashByID);
      string stationTo = userNeed.Abbrev;

      cout << "Trip: " << endl
	<< " ID: " << userTripEnq.TripID << endl
	<< " Starttime: " << userTripEnq.StartTime << endl
	<< " Bikeid: " << userTripEnq.BikeID << endl << " Duration: ";
      displayDuration (userTripEnq.SecDuration);
      cout << endl
	<< " From station: " << stationFrom << " (" << userTripEnq.
	StationFrom << ") " << endl << " To station: " << stationTo << " (" <<
	userTripEnq.
	StationTo << ") " << endl << " Rider identifies as: " << userTripEnq.
	Gender << endl << " Birthyear: " << userTripEnq.
	BirthYear << endl << endl;
    }
  else
    {
      cout << "Trip not found" << endl << endl;
    }
}

//
// This function prints out the bikeData when it is being looked up
//
void
printBikeInfo (string userInput, int &userBike, hashmap < string,
	       int >bikesByID)
{
  if (bikesByID.search (userInput, userBike, HashByBike))
    {

      cout << "Bike:" << endl
	<< " ID: " << userInput << endl
	<< " Usage: " << userBike << endl << endl;
    }
  else if (!bikesByID.search (userInput, userBike, HashByBike))
    {
      cout << "Bike not found" << endl << endl;
    }
}

//
// This function checks to see if the input is valid for searching for a station's data by Abbrev
//
void
printStationAbbrevLookUp (string userInput, hashmap < string,
			  StationInfo > stationByAbbrev, StationInfo userNeed)
{
  if (stationByAbbrev.search (userInput, userNeed, HashByAbbrev))
    {
      printStationInfo (userNeed, userInput);
    }
  else
    {
      cout << "Station not found" << endl << endl;
    }
}

//
// This function checks to see if the input is valid for searching for a station's data by ID
//
void
printStationByID (hashmap < int, StationInfo > stationByID, string userInput,
		  StationInfo userNeed)
{
  if ((stationByID.search (stoi (userInput), userNeed, HashByID)))
    {
      printStationInfo (userNeed, userInput);
    }
  else
    {
      cout << "Station not found" << endl << endl;
    }
}




int main()
{
  string filename1, filename2;
  int numberOfBikes = 0;

  // Intro to program message and choosing files to deal with
  // in the program
  cout << "** DIVVY analysis program **" << endl << endl;
  cout << "Enter stations file> ";
  getline (cin, filename1);
  cout << "Enter trips file> ";
  getline (cin, filename2);
  cout << endl;

  // declaring the 4 hashmaps to store the stationData to search
  // by ID, by Abbreviation, info about all trips and info about
  // all bikes
  hashmap < int, StationInfo > stationByID (10000);
  hashmap < string, StationInfo > stationByAbbrev (10000);
  hashmap < string, TripInfo > tripsByID (2500000);
  hashmap < string, int >bikesByID (50000);

  // Reading in the data into the 4 hashmaps
  int stationCount =
    inputStationData (filename1, stationByID, stationByAbbrev);
  int tripCount =
    inputTripData (filename2, tripsByID, bikesByID, numberOfBikes);

  // Outputs the data based on number of entries in the hashmap
  cout << endl << "# of stations: " << stationCount << endl;
  cout << "# of trips: " << tripCount << endl;
  cout << "# of bikes: " << numberOfBikes << endl << endl;

  // declaring variables for handling userInput
  // and retrieving data from search
  string userInput;
  StationInfo userNeed;
  TripInfo userTripEnq;
  string nearby = "nearby";
  string similar = "similar";
  int userBike = 0;
  cout << "Please enter a command, help, or #> ";
  getline (cin, userInput);


  while (userInput != "#")
    {

    if (userInput == "help")
    {
      help ();
    }

  else if (isNumeric (userInput))
    {
      printStationByID (stationByID, userInput, userNeed);
    }

  else if (userInput.at (0) == 'B')
    {
      
      printBikeInfo (userInput, userBike, bikesByID);
    }

  else if (userInput.at (0) == 'n' && userInput.at (1) == 'e'
	   && userInput.at (2) == 'a' && userInput.at (5) == 'y')
    {

      nearbyExecution (stationByID, userInput);
    }

  else if (userInput.at (0) == 's' && userInput.at (1) == 'i'
	   && userInput.at (2) == 'm' && userInput.at (6) == 'r')
    {

      similarExecution (tripsByID, stationByID, userInput);

    }
  else if (userInput.at (0) == 'T' && userInput.at (1) == 'r')
    {

      printTripInfo (tripsByID, userTripEnq, stationByID, userInput,
		     userNeed);
    }
  else
    {
      printStationAbbrevLookUp (userInput, stationByAbbrev, userNeed);
    }

      cout << "Please enter a command, help, or #> ";
      getline (cin, userInput);

    }

}
