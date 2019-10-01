/*
Written by Daniel kevin Blackley - 160007726
*/
#ifndef MY_HASH_TABLE
#define MY_HASH_TABLE

#include "HashNode.h"
#include <vector>
#include <list>

using namespace std;

//errors to be thrown
enum HashTableError { OUT_OF_MEMORY, KEY_NOT_FOUND, DUPLICATE_KEY };

typedef unsigned long ulint;

class HashTable {
  typedef vector <list<HashNode> > Table;
  Table *table; // size of table is stored in the Table data structure
  size_t num;   // number of entries in the HashTable;

public:
  HashTable() { // constructor, initializes table of size 11;

    //try/catch for memory allocation
    try {
      table = new Table(11);

    } catch(bad_alloc& execption) {
        cerr << "Memory Allocation Failure" << endl;
        throw OUT_OF_MEMORY;
    }
    //set number of entries to 0
    num = 0;
  }


  HashTable(size_t hashSize) { // constructor, requires size of table as arg

    try {

      table = new Table(hashSize);

    } catch(bad_alloc& execption) {
        cerr << "Memory Allocation Failure" << endl;
        throw OUT_OF_MEMORY;
    }

    num = 0;
  }


  ~HashTable() {}      // empty deconstructor

  size_t size() { return table->size(); } // returns size of the hash table (number of buckets)

  /*
  hahs function that supposodly provides and even distribution of numbers, this method
  takes inspiration from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
  */
  size_t hash_function(ulint key) {
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;

    return key % table->size();
  }

  /*
  The following method gets the value from the hashtable that is related to the current key
  will through a KEY_NOT_FOUND error if the key doesn't exist within the table
  */
  ulint getValue(ulint key) {

    size_t bucket = hash_function(key);
    list <HashNode> *hashList = &table->at(bucket);

    //iterator to iterate through the list
    for (list<HashNode>::iterator hashIterator = hashList->begin(); hashIterator != hashList->end(); hashIterator++) {

      if (hashIterator->getKey() == key) {

        return hashIterator->getValue();
      }
    }
    throw KEY_NOT_FOUND;
    //return statement that should, in theory, never be reached
    return -1;
  }

  /*
  The following method checks the load factor and can rehash the table  if the table becomes too large
  It is also responsible fo entering the relevant key and data into the table. Throws
  a DUPLICATE_KEY error if the key already exists in the table
  */
  void insert(ulint key, ulint data) {  // insert data associated with key into table

    //if the load factor is too large, rehash the table
    if ((float) num/(table->size()) > 0.75) {
      //double the size of the table for rehashing
      rehash(2*table->size());
    }

    size_t bucket = hash_function(key);
    list <HashNode> *hashList = &table->at(bucket);

    //iterator to check is a DUPLICATE_KEY exists
    for (list<HashNode>::iterator hashIterator = hashList->begin(); hashIterator != hashList->end(); hashIterator++) {

      if (hashIterator->getKey() == key) {
        throw DUPLICATE_KEY;
      }
    }
    //increase number of items in the table
    this->num++;
    hashList->push_back(HashNode(key, data));
  }

  /*
  remove an item from the table which matches the key, throws a KEY_NOT_FOUND error if
  the key doesn't exist
  }
  */
  void erase(ulint key) {    // remove key and associated data from table

    size_t bucket = hash_function(key);
    list <HashNode> *hashList = &table->at(bucket);

    size_t prevNum = this->num;


    for (list<HashNode>::iterator hashIterator = hashList->begin(); hashIterator != hashList->end(); hashIterator++) {

      if (hashIterator->getKey() == key) {

        hashIterator = hashList->erase(hashIterator);
        this->num--;
      }
    }

    //if no key was removed
    if (prevNum == this->num) {
      throw KEY_NOT_FOUND;
    }
  }

  /*
  This method rehashes the table, using a vector to store all the Hashnodes in the old
  table then adding them into the new table, throws OUT_OF_MEMORY if memory runs out
  */
  void rehash(size_t newSize) {

    vector<HashNode> hashStorage;
    list<HashNode> tempList;
    ulint key, data;

    for (size_t i = 0; i < table->size(); i++) {

      tempList = table->at(i);

      //cycle through the nodes from the old table and store them
      for (list<HashNode>::iterator hashIterator = tempList.begin(); hashIterator != tempList.end(); hashIterator++) {

        key = hashIterator->getKey();
        data = hashIterator->getValue();

        hashStorage.push_back(HashNode(key, data));

      }
    }

    //reset number of entries in the table
    this->num = 0;

    delete this->table;

    try {
      this->table = new Table(newSize);

    } catch(bad_alloc& execption) {

        cerr << "Memory Allocation Failure" << endl;
        throw OUT_OF_MEMORY;
    }

    //cycle through the nodes from the old table and reinster them
    for (vector<HashNode>::iterator vecIterator = hashStorage.begin() ; vecIterator != hashStorage.end(); ++vecIterator) {

      key = vecIterator->getKey();

      data = vecIterator->getValue();

      this->insert(key, data);
    }
  }
};

#endif
