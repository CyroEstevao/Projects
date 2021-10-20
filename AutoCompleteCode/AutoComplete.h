#ifndef EMPTY_H__
#define EMPTY_H__

#include <string>
#include <iostream>
using namespace std;

const int Asize= 26;
string key="";
// struct TrieNode {
//   struct 
  


// };

class Tries{

public:
//constructor
  Tries* child[Asize];
  
  bool isLeaf;
  Tries();
  
  ~Tries();

  // int EmptyFunc();

  
  //initializes a node
  // shared_ptr<TrieNode> InitNode();

  //A bool function that points if a node is a leaf
  // bool isLeaf();

  //A function that will insert a new word into the tree
  void Insert(string key);

  //A funtion that will remove a word from the tree and return true 
  //for deleted or false not found
  bool Remove();

  //This function checks if a word is in the tree
  bool LookUp(string Key);

  //This function prints suggestions for a given prefix
  int AutoComplete();


private:
  int empty_count;
};

#endif // EMPTY_H__