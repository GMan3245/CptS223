#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include "Employee.h"
#include "utils.h"

using namespace std;

// this inplementation follows Figure 5.14 in textbook for quadratic probing
template <typename HashedObj> 
class ProbingHash
{
  public:
    explicit ProbingHash( int size = 101 ) : array( nextPrime( size ) ), currentSize{ 0 }
      { makeEmpty( ); }

    bool contains( const HashedObj & x ) const
    {
        return isActive(findPos(x));
    }

    void makeEmpty() {
        currentSize = 0;
        for (auto& entry : array) {
            entry.info = EMPTY;  // Mark each entry as EMPTY
        }
    }


    bool insert( const HashedObj & x )
    {
        int currentPos = findPos(x);

        // if x is already in the table, do not insert it again
        if (isActive(currentPos)) {
            return false;
        }

        // insert x at the found position and mark it as active
        array[currentPos].element = x;
        array[currentPos].info = ACTIVE;
        ++currentSize;  // Increment currentSize after insertion

        // Check if we need to rehash (when load factor > 0.5)
        if (currentSize > array.size() / 2) {
            rehash();
        }

        return true;
    }
    
    bool insert( HashedObj && x )
    {
        int currentPos = findPos(x);

        // if x is already in the table, do not insert it again
        if (isActive(currentPos)) {
            return false;
        }

        // insert x at the found position using move semantics and mark it as active
        array[currentPos].element = std::move(x);
        array[currentPos].info = ACTIVE;
        ++currentSize;  // Increment currentSize after insertion

        // Check if we need to rehash (when load factor > 0.5)
        if (currentSize > array.size() / 2) {
            rehash();
        }

        return true;
    }

    bool remove( const HashedObj & x )
    {
        int currentPos = findPos(x);
        if(!isActive(currentPos)) {
            return false;
        }

        //mark the entry as deleted and decrement the active element count
        array[currentPos].info = DELETED;
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
        return array.size();
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

  private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj{ }, EntryType i = EMPTY )
          : element{ e }, info{ i } { }
        
        HashEntry( HashedObj && e, EntryType i = EMPTY )
          : element{ std::move( e ) }, info{ i } { }
    };
    
    vector<HashEntry> array;
    int currentSize;

    bool isActive( int currentPos ) const
      { return array[ currentPos ].info == ACTIVE; }

    int findPos(const HashedObj& x) const {
        int offset = 1;
        int currentPos = myhash(x);

        // Continue probing while the position is not empty and does not contain x
        while (array[currentPos].info != EMPTY && array[currentPos].element != x) {
            currentPos += offset;   // Compute the next probe position
            offset += 2;            // Increment the offset for quadratic probing

            // Wrap around the array if currentPos goes out of bounds
            if (currentPos >= array.size()) {
                currentPos -= array.size();
            }
        }
        return currentPos;
    }

    void rehash() {
        // Store the old array temporarily
        vector<HashEntry> oldArray = array;

        // Resize the array to the next prime size that is roughly double the current size
        array.resize(nextPrime(2 * oldArray.size()));
        for (auto& entry : array) {
            entry.info = EMPTY;  // Mark each entry as EMPTY in the new array
        }

        // Reset currentSize to zero since we are going to reinsert elements
        currentSize = 0;

        // reinsert all active elements from the old array into the new array
        for (auto& entry : oldArray) {
            if (entry.info == ACTIVE) {
                insert(std::move(entry.element));  //move the element, not the entire entry
            }
        }
    }


    size_t myhash( const HashedObj & x ) const
    {
        static std::hash<HashedObj> hf;
        return hf( x ) % array.size( );
    }


    double loadFactor()
    {
        return static_cast<double>(currentSize) / array.size();
    }
};

#endif
