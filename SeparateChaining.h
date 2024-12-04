#ifndef SEPARATE_CHAINING_H
#define SEPARATE_CHAINING_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include "Employee.h"
#include "utils.h"

using namespace std;

template <typename HashedObj>
class ChainingHash
{
  public:
    explicit ChainingHash( int size = 101 ) : currentSize{ 0 }
      { theLists.resize( 101 ); }

    bool contains( const HashedObj & x ) const
    {
        const auto & whichList = theLists[myhash( x )];
        return std::find(begin(whichList),end(whichList), x) != end(whichList);
    }

    void makeEmpty( )
    {
        currentSize = 0;
        for (auto& entryList : theLists) {
            entryList.clear();  // Clear each list in the vector
        }
    }

    bool insert( const HashedObj & x ) //L Value Insert
    {
        auto & whichList = theLists[myhash( x )]; //gets the list for the hash value of x

        //checks if x is already in the list to avoid duplicates
        if(std::find(begin(whichList),end(whichList), x)!= end(whichList)) {
            return false; //x already exists, so no insertion is done
        }
        whichList.push_back( x ); //insert x at the end of the list
        // this "insert" function accepts Lvalues

        //increments the currentSIze and checks if the rehashing is needed
        if(++currentSize > theLists.size()) {
            rehash();
        }

        return true;
    }
    
    bool insert( HashedObj && x ) //R value insert
    {
        auto & whichList = theLists[myhash( x )]; //gets the list for the hash value of x

        //checks if x is already in the list to avoid duplicates
        if(std::find(begin(whichList),end(whichList), x)!= end(whichList)) {
            return false; //x already exists, so no insertion is done
        }
        whichList.push_back(std::move( x )); //insert x at the end of the list using the move
        // this "insert" function accepts Lvalues

        //increments the currentSIze and checks if the rehashing is needed
        if(++currentSize > theLists.size()) {
            rehash();
        }

        return true;
    }

    bool remove( const HashedObj & x )
    {

        auto & whichList = theLists[myhash( x )];//gets the list corresponding to xs hash

        //find x in the target list
        auto itr = find(begin(whichList), end(whichList), x);

        if (itr == end(whichList))    //if x is not found, return false
            return false;

        whichList.erase(itr); //remove x from the list to decrease currentsize
        --currentSize;

        return true;
    }

    double readLoadFactor() 
    {
        return loadFactor();
    }

    double readCurrentSize() 
    {
        return currentSize;
    }

    double readArraySize() 
    {
        return theLists.size();
    }

  private:

    //vector<list<HashedObj>> theLists;   // The array of Lists
    std::vector<std::list<HashedObj>> theLists;
    int currentSize;

    void rehash( )
    {
        //store the old list array temporarily
        auto oldLists = theLists;

        //resize theLists to the next prime number thats roughly double the current size
        theLists.resize(nextPrime(2*theLists.size()));

        //clear new list
        for (auto & list : theLists)
            list.clear();

        //resent the currentSize to 0 so it can reinsert elements
        currentSize = 0;


        for( auto & list : oldLists )
            for (auto& x : list)
                insert(std::move(x));
    }

    size_t myhash(const HashedObj& x) const {
        static std::hash<HashedObj> hf;
        return hf(x) % theLists.size();
    }


    double loadFactor(){
        return static_cast<double>(currentSize) / theLists.size();
    }
};

#endif
