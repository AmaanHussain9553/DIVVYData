/*hashmap.h*/

//
// <<YOUR NAME>>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06: hashing
//
// References: original non-collision code written by Prof. Hummel (UIC)
// 
// hashmap implements a hash table assuming a perfect hash function;
// the class is not designed to handle collisions.  The class provides
// the underlying hash table along with insert() and search() functions;
// the user of this class must provided a Hash(key, N) function that 
// hashes the given key and returns an integer in the range 0..N-1, 
// inclusive, if the key is valid.  If the key is invalid, the Hash() 
// function must return a negative value.
//
// The hash function is provided in the calls to insert and search.
// Pass the name of your hash function as the 3rd parameter.  Example:
//
//   int myHash(string key, int N)
//   { ... }
//
//   int N = 10000;
//   hashmap<string, double>  hmap(N);  
//
//   if (hmap.insert("deep-dish pizza", 19.99, myHash))
//     cout << "inserted" << endl;
//   else
//     cout << "not inserted?!" << endl;
// 

#pragma once

#include <iostream>
#include <functional>

using namespace std;

template<typename KeyT, typename ValueT>
class hashmap
{
private:
  struct HashEntry
  {
    bool   ContainsData;  // false => empty, true => contains data
    KeyT   Key;           // (key, value)
    ValueT Value;

    HashEntry()
    {
      ContainsData = false;
    }
  };

  HashEntry* HT;  // hash table array of structs
  int        N;   // capacity of hash table (# of locations or array size)

public:
  //
  // constructor: N is the size of the underlying hash table
  //
  hashmap(int N)
  {
    this->N = N;
    this->HT = new HashEntry[N];
  }
  
  //
  // destructor
  //
  virtual ~hashmap()
  {
    delete[] this->HT;
  }

  //
  // getN
  //
  // Returns the capacity of the hash table, i.e. the value N that
  // was passed to the constructor.
  //
  int getN() const
  {
    return this->N;
  }
  
  //
  // _copy helper function
  //
  // This function takes in parameter of current hashtable and 
  // new hastable and copies all elements from the new hastable 
  // to the already emptied current hastable
  //
  void _copy(HashEntry* HT, HashEntry* other, int N){
      
      for(int i = 0; i < N; i++){
          if(other[i].ContainsData == true){
              HT[i].Key = other[i].Key;
              HT[i].Value = other[i].Value;
              HT[i].ContainsData = true;
          }
      }
  
  }
  
  //
  // copy constructor
  //
  // Returns a deep copy of the hashtable
  //
  hashmap(const hashmap& other){
      
 
      HT = NULL;
      N = other.N;
      this->HT = new HashEntry[N]; //creates new hashtable with the new size
      
      HashEntry* otherHT = other.HT;
      
      _copy(HT, otherHT, N); // helper function to copy all values from 
                             // new to current hashtable

  }
  
  //
  // operator= function
  //
  // This function deletes the current hashtable and sets the 
  // new hashtable to the current one
  //
  hashmap& operator=(const hashmap& other){
      
      delete[] this->HT; //deletes current hashtable
      N = other.N;
      this->HT = new HashEntry[N]; //creates new hashtable with the new size
      
      HashEntry* otherHT = other.HT;
      
      _copy(HT, otherHT, N); // helper function to copy all values from 
                             // new to current hashtable
      
  }
  
  //
  // ReturnNumberFilled
  //
  // THis function was used to debug the operator=
  // function and the copy constructor making sure all 
  // keys and values were successfully copied by counting
  // all positions where there was data.
  //
  int returnNumberFilled(){
      int count = 0;
      for(int i = 0; i < N; i++){ //running through array
          if(HT[i].ContainsData == true){  // incrementing count when data is present
              count++;
          }
      }
      return count;
  }
  
  //
  // getVectorWithKeys 
  //
  // This function takes a vector as a parameter 
  // and stores all the keys of a hashmap in that
  // vector, helper function for nearby and similar
  // commands
  // 
  vector<KeyT> getVectorWithKeys(vector<KeyT> Keys){
 
      for(int i = 0; i < N; i++){
          if(this->HT[i].ContainsData == true){
              Keys.push_back(this->HT[i].Key);
          }
      }
      
      return Keys;
  }
  
  //
  // operator[]
  //
  // This function returns the values
  // for a particular element
  //
  ValueT operator[](int i){
      return HT[i].Value;
  }

  //
  // insert
  //
  // Inserts the given (key, value) pair into the hash table, overwriting
  // the previous value if already inserted.  If the insert was successful
  // then true is returned, otherwise false is returned (meaning the key
  // was not hashed successfully, e.g. due to improper formatting).
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling insert
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);  
  //
  //   if (hmap.insert("deep-dish pizza", 19.99, myHash))
  //     cout << "inserted" << endl;
  //   else
  //     cout << "not inserted?!" << endl;
  //
  bool insert(KeyT key, ValueT value, function<int(KeyT,int)> Hash)
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
    int index = Hash(key, N);
    int endSearch = (index - 1); //value to check if tried inserting 
                                 //through whole hastable

    //
    // insert:
    //
    if (index < 0 || index >= N)  // invalid key or hash function:
    {
      return false;
    }
    else{
            //if the index is already filled and making sure hasnt searched through whole array
            while(HT[index].ContainsData == true && HT[index].Key != key && index != endSearch){
                index++;
                if(index == N){
                        index = 0;  // wraps index to the top when reaches the end
                }
            }
   
            HT[index].Key = key;
            HT[index].Value = value;
            HT[index].ContainsData = true;  // array location now contains data

            return true; // value has been inserted
    }
  }

  //
  // search
  //
  // Searches the hash table for the matching key, and if found then
  // (1) the value is returned via the reference parameter and (2) true
  // is returned.  If the key could not be found, or if the hash 
  // function returned an invalid value (e.g. due to an improperly 
  // formatted key), then false is returned.
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling search
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);
  //   ...
  //   double price;
  //   if (hmap.search("deep-dish pizza", price, myHash))
  //     cout << "deep-dish piazza costs " << price << endl;
  //   else
  //     cout << "not found?!" << endl;
  //
  bool search(KeyT key, ValueT& value, function<int(KeyT,int)> Hash) const
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
    int index = Hash(key, N);
   
    //
    // search:
    //
    if (index < 0 || index >= N)  // invalid key or hash function:
    {
      return false;
    }
    
    else if (HT[index].ContainsData == true){ //else searching at the position 
                                              //where the key would originally be
            
        while(HT[index].ContainsData == true){  // hash table contains data:
          
          if(HT[index].Key == key){
          value = HT[index].Value;  // return value via reference parameter                                      
          return true;              // if the data key matches 
          }
              index++;              //else searching through array for the certain key
              if(index == N){
                  index = 0;        // wraps index to the top when reaches the end
              }
              
       }
   }
   else{
       return false; // value not found, so returns false
   }
   return false;
  }

};