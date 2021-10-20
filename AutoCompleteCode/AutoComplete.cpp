#include "Empty.h"
#include <bits/stdc++.h>
#include <string>
#include <iomanip>
#include <string>
#include <iostream>

using namespace std;

Tries::Tries(){
        this->isLeaf = true;
 
        for (int i = 0; i < Asize; i++) {
            this->child[i] = nullptr;
        }
}

Tries::~Tries(){
    
}


//   ~Empty();

//   int EmptyFunc();

  //initializes a node
// shared_ptr<TrieNode> InitNode(){
//     shared_ptr<TrieNode> ptr(new TrieNode);
//     ptr->isLeaf=true;
//     return ptr;
//   }

  


  //A bool function that points if a node is a leaf
//   bool isLeaf();

  //A function that will insert a new word into the tree
  void Tries::Insert(string key){
    Tries *cur = this;
    int a= key.length();
    for (int i = 0; i < a; i++){
        if (cur->child[key[i]] == nullptr) {

            cur->child[key[i]] = new Tries();
        }
         cur = cur->child[key[i]];
    }
    cur->isLeaf = true;
}

  //A funtion that will remove a word from the tree and return true 
  //for deleted or false not found
  bool Remove();

  //This function checks if a word is in the tree
  
  bool Tries::LookUp(string key){
    Tries* temp1 = this;
    int a=key.length();

    if (temp1 == nullptr){
        return false;
    }
    for (int i = 0; i < a; i++){

        temp1 = temp1->child[key[i]];
 
        if (temp1 == nullptr) {
            return false;
        }
    }
    return temp1->isLeaf;
}

  //This function prints suggestions for a given prefix
  int AutoComplete();

// int Empty::EmptyFunc(){
//     return 2;
// }
int main()
{
    Tries* head = new Tries();
    // string key= ;
    head->Insert("hello");
    cout << head->LookUp("hello") << " ";      // print 1
    // string key2=;
    head->Insert("helloworld");
    cout << head->LookUp("helloworld") << " "; // print 1
 
    cout << head->LookUp("helll") << " ";      // print 0 (Not found)
 
    head->Insert("hell");
    cout << head->LookUp("hell") << " ";       // print 1
 
    head->Insert("h");
    cout << head->LookUp("h");                 // print 1
 
    cout << endl;
    return 0;
}