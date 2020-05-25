/*test01.cpp*/

//
// Unit tests for threaded AVL tree
//

#include <iostream>
#include <vector>

#include "avlt.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("(1) empty tree")
{
  avlt<int, int>  tree;

  REQUIRE(tree.size() == 0);
  REQUIRE(tree.height() == -1);
}


TEST_CASE("(2) case 1 at the root--Right Rotation")
{
  avlt<int, int>  tree;

  vector<int> keys = { 100, 80, 60 };
  vector<int> heights = { 0, 1, 0 };

  for (int key : keys)
  {
    tree.insert(key, -key);
  }

  //
  // size and height?  after rebalance should be 1:
  //
  REQUIRE(tree.size() == keys.size());

  auto maxH = std::max_element(heights.begin(), heights.end());
  REQUIRE(tree.height() == *maxH);
  // 
  // values inserted?
  //
  int value;

  for (int key : keys)
  {
    REQUIRE(tree.search(key, value));
    REQUIRE(value == -key);
  }

  //
  // heights correct after rebalance?
  //
  for (size_t i = 0; i < keys.size(); ++i)
  {
    REQUIRE((tree % keys[i]) == heights[i]);
  }
}

TEST_CASE("(8) case 4: Rotate at root - Left rotation")
{
    avlt<int, int>  tree;
    
    vector<int> keys = { 30, 10, 96, 5, 15, 85, 110, 64, 90, 36 };
    vector<int> heights = { 3, 1, 1, 0, 0, 2, 0, 1, 0, 0 };
    
    for (int key : keys)
    {
      tree.insert(key, -key);
    }
    
    REQUIRE(tree.size() == keys.size());
    
    auto maxH = std::max_element(heights.begin(), heights.end());
    REQUIRE(tree.height() == *maxH);
    
    int value;

    for (int key : keys) //Check value insertion
    {
      REQUIRE(tree.search(key, value));
      REQUIRE(value == -key);
    }
    
    for (size_t i = 0; i < keys.size(); ++i) //Check the height after rebalance
    {
      REQUIRE((tree % keys[i]) == heights[i]);
    }
    
}

TEST_CASE("(3) case 2: Inserting to the right of k1 (Double rotation case 1) ") //Left Right
{
    avlt<int, int>  tree;
    
    vector<int> keys = { 55, 48, 64, 38, 51, 60, 78, 16, 40, 45 };
    vector<int> heights = { 3, 1, 1, 1, 0, 0, 0, 0, 2, 0 };
    
    for (int key : keys)
    {
      tree.insert(key, -key);
    }
    
    REQUIRE(tree.size() == keys.size());
    
    auto maxH = std::max_element(heights.begin(), heights.end());
    REQUIRE(tree.height() == *maxH);
    
    int value;

    for (int key : keys) //Check value insertion
    {
      REQUIRE(tree.search(key, value));
      REQUIRE(value == -key);
    }
    
    for (size_t i = 0; i < keys.size(); ++i) //Check the height after rebalance
    {
      REQUIRE((tree % keys[i]) == heights[i]);
    }
    
}

TEST_CASE("(4) case 3: Inserting to the left of k3 (Double rotation case 2) ") //Right Left
{
    avlt<int, int>  tree;
    
    vector<int> keys = { 100, 50, 150, 125, 175, 140 };
    vector<int> heights = { 1, 0, 1, 2, 0, 0 };
    
    for (int key : keys)
    {
      tree.insert(key, -key);
    }
    
    REQUIRE(tree.size() == keys.size());
    
    auto maxH = std::max_element(heights.begin(), heights.end());
    REQUIRE(tree.height() == *maxH);
    
    int value;

    for (int key : keys) //Check value insertion
    {
      REQUIRE(tree.search(key, value));
      REQUIRE(value == -key);
    }
    
    for (size_t i = 0; i < keys.size(); ++i) //Check the height after rebalance
    {
      REQUIRE((tree % keys[i]) == heights[i]);
    }
    
}

TEST_CASE("(5) case 4: Inserting to the right of k2--Left rotation ")
{
    avlt<int, int>  tree;
    
    vector<int> keys = { 100, 50, 150, 25, 75, 175, 200 };
    vector<int> heights = { 2, 1, 0, 0, 0, 1, 0 };
    
    for (int key : keys)
    {
      tree.insert(key, -key);
    }
    
    REQUIRE(tree.size() == keys.size());
    
    auto maxH = std::max_element(heights.begin(), heights.end());
    REQUIRE(tree.height() == *maxH);
    
    int value;

    for (int key : keys) //Check value insertion
    {
      REQUIRE(tree.search(key, value));
      REQUIRE(value == -key);
    }
    
    for (size_t i = 0; i < keys.size(); ++i) //Check the height after rebalance
    {
      REQUIRE((tree % keys[i]) == heights[i]);
    }
    
}

TEST_CASE("(6) case 1: Insert to left of k1- right rotation")
{
    avlt<int, int>  tree;
    
    vector<int> keys = { 100, 50, 150, 75, 25, 15 };
    vector<int> heights = { 1, 2, 0, 0, 1, 0 };
    
    for (int key : keys)
    {
      tree.insert(key, -key);
    }
    
    REQUIRE(tree.size() == keys.size());
    
    auto maxH = std::max_element(heights.begin(), heights.end());
    REQUIRE(tree.height() == *maxH);
    
    int value;

    for (int key : keys) //Check value insertion
    {
      REQUIRE(tree.search(key, value));
      REQUIRE(value == -key);
    }
    
    for (size_t i = 0; i < keys.size(); ++i) //Check the height after rebalance
    {
      REQUIRE((tree % keys[i]) == heights[i]);
    }
    
}

TEST_CASE("(7) Changing at root: Left rotation")
{
    avlt<int, int>  tree;
    
    vector<int> keys = { 100, 150, 175 };
    vector<int> heights = { 0, 1, 0 };
    
    for (int key : keys)
    {
      tree.insert(key, -key);
    }
    
    REQUIRE(tree.size() == keys.size());
    
    auto maxH = std::max_element(heights.begin(), heights.end());
    REQUIRE(tree.height() == *maxH);
    
    int value;

    for (int key : keys) //Check value insertion
    {
      REQUIRE(tree.search(key, value));
      REQUIRE(value == -key);
    }
    
    for (size_t i = 0; i < keys.size(); ++i) //Check the height after rebalance
    {
      REQUIRE((tree % keys[i]) == heights[i]);
    }
    
}

TEST_CASE("(9) Changing at root: Left rotation-- part 2")
{
    avlt<int, int>  tree;
    
    vector<int> keys = { 100, 50, 150, 125, 200, 175 };
    vector<int> heights = { 1, 0, 2, 0, 1, 0 };
    
    for (int key : keys)
    {
      tree.insert(key, -key);
    }
    
    REQUIRE(tree.size() == keys.size());
    
    auto maxH = std::max_element(heights.begin(), heights.end());
    REQUIRE(tree.height() == *maxH);
    
    int value;

    for (int key : keys) //Check value insertion
    {
      REQUIRE(tree.search(key, value));
      REQUIRE(value == -key);
    }
    
    for (size_t i = 0; i < keys.size(); ++i) //Check the height after rebalance
    {
      REQUIRE((tree % keys[i]) == heights[i]);
    }
    
}