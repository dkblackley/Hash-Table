/*
Written by Daniel kevin Blackley - 160007726
*/
#ifndef MY_HASH_NODE
#define MY_HASH_NODE

using namespace std;

class HashNode {
  unsigned long key;   // The hash node's key
  unsigned long value; // The key's associated data


public:

  //constructer sets initial value for key and data
  HashNode() {
    this->key = 0;
    this->value = 0;
  }

  //alternative constructor that assigns values as taken in
  HashNode(unsigned long key, unsigned long value) {
    this->key = key;
    this->value = value;
  }

  //getters
  unsigned long getKey() { return key; }
  unsigned long getValue() { return value; }

  //assign both the key and value
  void assign(unsigned long key, unsigned long value) {
    this->key = key;
    this->value = value;
  }
};

#endif
