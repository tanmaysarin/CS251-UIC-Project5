/*avlt.h*/

// Name  : Tanmay Sarin
// Class : CS251: Data Structures, Project 5
// University : University of Illinois at Chicago
// 
// Description: 
// Threaded AVL tree Implementation
// AVL trees as in self balancing trees.
//

#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cmath>

using namespace std;

template<typename KeyT, typename ValueT>
class avlt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded; // true => Right is a thread, false => non-threaded
    int    Height;     // height of tree rooted at this node
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* ptr = nullptr; //pointer to copy the node data from the begin function to the next function
  
public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  avlt()
  {
    Root = nullptr;
    Size = 0;
  }
  
  //
  // helper functions to return the Left and Right pointers; in
  // particular the ptr returned for the Right is controlled by 
  // the "isThreaded" field:  if "isThreaded" is true then nullptr
  // pointer is returned, otherwise the actual underyling ptr
  // is returned.
  //
  NODE* _getActualLeft(NODE* cur) const
  {
    return cur->Left;
  }

  NODE* _getActualRight(NODE* cur) const
  {
    if (cur->isThreaded)  // then actual Right ptr is null:
      return nullptr;
    else  // actual Right is contents of Right ptr:
      return cur->Right;
  }

  //
  // _copy
  //
  // Makes a copy of "othercur" into "this" tree.
  //
  void _copy(NODE* &cur, NODE* other)
  {
    if(other == nullptr){
            return;
        }      
    if(other != nullptr){
        NODE *node = new NODE();
        node->Key = other->Key;
        node->Value = other->Value;
        node->Height = other->Height;
        node->isThreaded = other->isThreaded;
        node->Left = other->Left;
        node->Right = other->Right;
        cur = node;

        _copy(node->Left,other->Left);
        if(other->isThreaded == false){
            _copy(node->Right, other->Right);
        }
    }   
  }

  //
  // copy constructor
  //
  // NOTE: makes an exact copy of the "other" tree, such that making the
  // copy requires no rotations.
  //
  avlt (const avlt& other)
  {
    Root = NULL;
    Size = other.Size;
    ptr = nullptr;

    _copy(Root, other.Root);  // to be safe, copy this state as well:
  }
  
  //
  //destroy:
  //
  //Helper function to delete the nodes and free up the space
  //
  void destroy(NODE* cur)
  {
      if(cur == NULL) 
          return;
      else{
          destroy(cur->Left);
          if(cur->isThreaded == true){
              cur->Right = nullptr;
          }
          destroy(cur->Right);
          delete cur;
      }
  }

  //
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~avlt()
  {
    destroy(Root);
  }

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  // NOTE: makes an exact copy of the "other" tree, such that making the
  // copy requires no rotations.
  //
  avlt& operator=(const avlt& other)
  {
    
    clear();
    //
    // now copy the other one:
    //
    _copy(Root, other.Root);
    Size = other.Size;

    this->ptr = other.ptr;  // to be safe, copy this state as well:

    return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
    destroy(Root);
    Size = 0;
    Root = NULL;
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }

  // 
  // height:
  //
  // Returns the height of the tree, -1 if empty.
  //
  // Time complexity:  O(1) 
  //
  int height() const
  {
    if (Root == nullptr)
      return -1;
    else
      return Root->Height;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) worst-case
  //
  bool search(KeyT key, ValueT& value) const
  {
    NODE* cur = Root;

      while (cur != nullptr)
      {
        if (key == cur->Key){ // already in tree
            value = cur->Value;
            return true;
        }  
          
        if (key < cur->Key)  // search left:
        {
          cur = cur->Left;
        }
        else //searches right
        {
            if(cur->isThreaded){ //if the right is threaded
                cur = nullptr; //declare the right as null ptr and stop the transversal
            }else{ //if right is not threaded
                cur = cur->Right;
            }
        }
      }//while  
  
      // if get here, not found
      return false;
  }
  
  // 
  // search_helper:
  //
  // Helper function for the range_search
  // Find the lower key in the avl tree
  // and returns true if found
  //
  // Time complexity:  O(lgN) worst-case
  //
  bool search_helper(KeyT key)
  {
    NODE* cur = Root;

      while (cur != nullptr)
      {
        if (key == cur->Key){ // already in tree
            return true;
        }  
          
        if (key < cur->Key)  // search left:
        {
          cur = cur->Left;
        }
        else //searches right
        {
            if(cur->isThreaded){ //if the right is threaded
                cur = nullptr; //declare the right as null ptr and stop the transversal
            }else{ //if right is not threaded
                cur = cur->Right;
            }
        }
      }//while  
  
      // if get here, not found
      return false;
  }

  //
  // range_search
  //
  // Searches the tree for all keys in the range [lower..upper], inclusive.
  // It is assumed that lower <= upper.  The keys are returned in a vector;
  // if no keys are found, then the returned vector is empty.
  //
  // Time complexity: O(lgN + M), where M is the # of keys in the range
  // [lower..upper], inclusive.
  //
  // NOTE: do not simply traverse the entire tree and select the keys
  // that fall within the range.  That would be O(N), and thus invalid.
  // Be smarter, you have the technology.
  //
  vector<KeyT> range_search(KeyT lower, KeyT upper)
  {
    vector<KeyT>  keys;
    
    while(lower <= upper){
        if(search_helper(lower) == true){
            keys.push_back(lower);
            lower++; //increase the lower limit
        }else{
            lower++; //increase the lower limit
        }
    }
    return keys; //return the vector
  }
  
  //
  // Helper functions to get the heights 
  // of various nodes that are used in
  // the insert function, etc.
  // 
  int heightHelper(NODE* A){
      if(A == nullptr)
          return -1;
      else
         return A->Height;
  }
  
  int heightRight(NODE* A){
      if(A->isThreaded == true)
          return -1;
      else
          return A->Right->Height;
  }

  //
  // _RightRotate
  //
  // Rotates the tree around the node N, where Parent is N's parent.  Note that
  // Parent could be null, which means N is the root of the entire tree.  If 
  // Parent denotes a node, note that N could be to the left of Parent, or to
  // the right.  You'll need to take all this into account when linking in the
  // new root after the rotation. Update the heights and the threads as well.
  //
   void rightRotate(NODE* Parent, NODE* N)
  {
     // 1) Lable the components
     // 2) rotate
     // 3) update parent to link to L
     // 4) update N's height
     // 5) update L's height
     
     NODE* L = N->Left; // Step 1
     NODE* B = L->Right;
     
     if(L->isThreaded){
         B = nullptr;
     }
     
     N->Left = B; //Step 2
     L->Right = N;
     L->isThreaded = false;
     
     if(Parent == nullptr){ //Step 3
        Root = L;
     }
     else if (L->Key < Parent->Key){
        Parent->Left = L;
     }
     else{
        Parent->Right = L;
     }
     
     N->Height = 1 + max(heightHelper(N->Left), heightRight(N)); //Step 4
     L->Height = 1 + max(heightHelper(L->Left), heightRight(L)); //Step 5
  }
  
  //
  // _LeftRotate
  //
  // Rotates the tree around the node N, where Parent is N's parent.  Note that
  // Parent could be null, which means N is the root of the entire tree.  If 
  // Parent denotes a node, note that N could be to the left of Parent, or to
  // the right.  You'll need to take all this into account when linking in the
  // new root after the rotation. Update the heights and the threads as well.
  //
  void leftRotate(NODE* Parent, NODE* N)
  {
     // 1) Lable the components
     // 2) rotate
     // 3) update parent to link to L
     // 4) update N's height
     // 5) update R's height
     
     NODE* R = N->Right; //Step 1
     NODE* B = R->Left;

     R->Left = N; //Step 2
     N->Right = B;
     if(B == nullptr){
         N->Right = R;
         N->isThreaded = true;
     }

     if(Parent == nullptr){ //Step 3
        Root = R;
     }
     else if (Parent->Right  == N){
        Parent->Right = R;
     }
     else{
        Parent->Left = R;
     }
    
     N->Height = 1 + max(heightHelper(N->Left), heightRight(N));
     R->Height = 1 + max(N->Height, heightRight(R));
  }

  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.  Rotations are performed
  // as necessary to keep the tree balanced according to AVL definition.
  //
  // Time complexity:  O(lgN) worst-case
  //
  void insert(KeyT key, ValueT value)
  {
    NODE* prev = nullptr;
    NODE* cur = Root;
    
    //
    // stack the nodes we visit so we can walk back up
    // the search path later, adjusting heights:
    //
    stack<NODE*> nodes;

    //
    // 1. Search to see if tree already contains key, and push the nodes on to the stack
    //
    while (cur != nullptr)
    {
      if (key == cur->Key)  // the key is in current/root
        return;
        
      nodes.push(cur);  // stack so we can return later:
      
      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else
      {
        prev = cur;
        if(cur->isThreaded) //if the right is threaded
            cur = nullptr; //stop the transversal, meaning the thread is not there
        else //not threaded, meaning there is a node to go to
            cur = cur->Right;
      }
    }//while
    
    //
    // 2. if we get here, key is not in tree, so allocate
    // a new node to insert:
    // 
    
    NODE* newNode = new NODE();
    newNode->Key = key;
    newNode->Value = value;
    newNode->Height = 0;
    newNode->Left = nullptr;
    newNode->Right = nullptr;
    newNode->isThreaded = true; //Mark all nodes as having a thread..
    //
    // 2.2 link in the new node:
    //
    // NOTE: cur is null, and prev denotes node where
    // we fell out of the tree.  if prev is null, then
    // the tree is empty and the Root pointer needs 
    // to be updated.
    //

      if(prev == nullptr)
      {
         Root = newNode;  
      } 
      
      //Whenever we add a new node, we only have to worry about the right node of the new node...
      
      else if(key < prev->Key) //When we are adding a left node, we only have to worry about the right node of the new node
      {
          prev->Left = newNode;//declare the new node
          newNode->Right = prev; //make a thread to connect the right node of the new ptr to the parent node...
         
      }
      else //When we are adding the right node, we only have to worry about the right node of the new node
      {
         newNode->Right = prev->Right; // prev->right is a thread pointing to its parent
                                       //We connect the right pointer from the new node
                                       //to the "old thread pointer" of its parent.
         prev->isThreaded = false; //Mark the thread of the previous node to be non-existant now
         prev->Right = newNode; //declare the new node..
      }
      
      // #. Increment the size
      Size++;
      
      while(!nodes.empty()){
           // 4.a cur = top of stack
           cur = nodes.top();
           
           nodes.pop();
           
           // 4.b compute new height of cur
           int HL, HR, HC, BF;

           HL = heightHelper(cur->Left);

           HR = heightRight(cur);

           HC = 1 + std::max(HL, HR);
           
           BF = HL - HR;
           
           // 4.c if height is same, exit loop
           if(HC == cur->Height){
               break;
           }else{
               cur->Height = HC;
           }
           NODE* parent  = nullptr;
           if(!nodes.empty()){
               parent = nodes.top();
           }
           
           if(abs(BF) > 1){
               if(HR > HL){
                   int HRL, HRR;

                   HRL = heightHelper(cur->Right->Left);
                   
                   HRR = heightRight(cur->Right);
                   
                   // right right case
                   if(HRR > HRL){
                       leftRotate(parent, cur);
                   }
                   else{
                       rightRotate(cur, cur->Right);
                       leftRotate(parent, cur);
                   }
               }
               else{
                   int HLL = -1, HLR = -1;

                   HLL = heightHelper(cur->Left->Left);
                   
                   HLR = heightRight(cur->Left);
                   
                   //left left case
                   if(HLL > HLR){
                       rightRotate(parent, cur);
                   }
                   //left right case
                   else{
                       leftRotate(cur, cur->Left);
                       rightRotate(parent, cur);
                   }
               }
           }
       }
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) worst-case
  //
  ValueT operator[](KeyT key) const
  {
    NODE* prev = nullptr;
    NODE* cur = Root;
    
    while (cur != nullptr)
    {
      if (key == cur->Key)  // the key is in current/root
        return cur->Value;

      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else
      {
        prev = cur;
        if(cur->isThreaded) //if the right is threaded
            cur = nullptr; //stop the transversal, meaning the thread is not there
        else //not threaded, meaning there is a node to go to
            cur = cur->Right;
      }
    }//while
    return ValueT{ };
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) worst-case
  //
  KeyT operator()(KeyT key) const
  {
    NODE* cur = Root;
    
    while (cur != nullptr)
    {
      if (key == cur->Key){  // the key is in current/root, i.e the key has been found...
      
          if(cur->isThreaded == true && cur->Right == nullptr){
              return KeyT{ }; //no right key exists, then break and return the default value
          }
          else if(cur->isThreaded == true && cur->Right != nullptr){
              cur = cur->Right; //transverse the thread to go to they key connected with the thread...
              return cur->Key; //return the key at the threaded link
          }
          else if (cur->isThreaded == false){
              cur = cur->Right;
              return cur->Key; //if there is no thread then just return the key to the right of the required key 
          }
      }

      if (key < cur->Key)  // search left:
      {
          cur = cur->Left;
      }
      else
      {
          if(cur->isThreaded) //if the right is threaded
              cur = nullptr; //stop the transversal, meaning the thread is not there
          else //not threaded, meaning there is a node to go to
              cur = cur->Right;
      }
    }//while

    return KeyT{ };
  }

  //
  // %
  //
  // Returns the height stored in the node that contains key; if key is
  // not found, -1 is returned.
  //
  // Example:  cout << tree%12345 << endl;
  //
  // Time complexity:  O(lgN) worst-case
  //
  int operator%(KeyT key) const
  {
    NODE* cur = Root;

    while (cur != nullptr)
    {
      if (key == cur->Key){ // already in tree
          return cur->Height;
      }  
        
      if (key < cur->Key)  // search left:
      {
        cur = cur->Left;
      }
      else //searches right
      {
          if(cur->isThreaded){ //if the right is threaded
              cur = nullptr; //declare the right as null ptr and stop the transversal
          }else{ //if right is not threaded
              cur = cur->Right;
          }
      }
    }//while  
  
    // if get here, not found
    return -1;
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) worst-case
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
    NODE* cur = Root;
     
    if(cur == nullptr){ //check if the current node is null or not
        ptr = cur; // copy the data of the cur node to the ptr node
        return;
    }
    
    while(cur->Left != NULL){
        cur = cur->Left;
    }
    
    ptr = cur; //copy the data of the cur node to the ptr node
  }

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) worst-case
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
    NODE* cur = ptr;
    // base case
    if(cur == nullptr){
        return false;
    }else{
    
        key = cur->Key; //return the key
        
        //Increment the pointer to the next inorder key
        if(cur->Left == NULL && cur->isThreaded == true){
            cur = cur->Right;
        }
        else if(cur->Left == NULL && cur->isThreaded == false){
            cur = cur->Right;
            while(cur->Left != NULL){ //check if anything is on the left, if yes, then go there
                cur = cur->Left;
            }
        }
        else if(cur->Left != NULL && cur->isThreaded == false){
                cur = cur->Right; //go to right node
                while(cur->Left != NULL){ //check if there is anything on the left
                    cur = cur->Left;
                }
        }
        else if(cur->Left != NULL && cur->isThreaded == true){

                cur = cur->Right;
        }

        ptr = cur; //copy the data of the cur node to the ptr node
        return true;
        
    }
    return false;
  }
  
  //
  // printInOrder:
  //
  // Helper function to print out
  // the keys in the dump function
  // in order.
  //
  void printInOrder(NODE* cur, ostream& output)const {
      if(cur == NULL){
          return;
      }
      else{
          printInOrder(cur->Left, output);
          
          if(cur->isThreaded == true && cur->Right != nullptr){
              output <<"(" << cur->Key << "," << cur->Value << "," << cur->Height << "," << cur->Right->Key  << ")" << endl;
              cur->Right = NULL;
          }else{
              output <<"(" << cur->Key << "," << cur->Value << "," << cur->Height << ")" << endl;
          }
          
          printInOrder(cur->Right, output);
      }
   }

  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* AVLT ***********************" << endl;

    output << "** size: " << this->size() << endl;
    output << "** height: " << this->height() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value,height) or (key,value,height,THREAD)
    //
    // (key,value,height) if the node is not threaded OR thread==nullptr
    // (key,value,height,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //

    printInOrder(Root,output);

    output << "**************************************************" << endl;
  }
	
};

