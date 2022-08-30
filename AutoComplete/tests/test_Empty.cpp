// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/Empty.h"

#include <iostream>
#include <string>

using namespace std;

class test_Empty : public ::testing::Test {
protected:
	// This function runs only once before any TEST_F function
	static void SetUpTestCase(){
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){
	}
    
	// this function runs before every TEST_F function
	void SetUp() override {
    }

	// this function runs after every TEST_F function
	void TearDown() override {
	}
};

TEST_F(test_Empty, TestEmptyFunc1){
    Empty myobj;
    
    ASSERT_EQ(2, myobj.EmptyFunc());
}

TEST_F(test_Empty, TestEmptyFunc2){
    Empty myobj;
    
    ASSERT_EQ(3, myobj.EmptyFunc());
}