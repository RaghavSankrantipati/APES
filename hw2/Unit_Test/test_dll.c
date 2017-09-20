/**************************************************************************************
*@Filename:test_dll.c
*
*@Description: This is an implementation of CMOCKA unit tests.
*It unit tests every functionality offered by dll.c
*@Author:Sai Raghavendra Sankrantipati
*@Date:19/07/2017
*@compiler:gcc
**************************************************************************************/


#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include  <stdlib.h>
#include "dll.h"

static void test_addNode(void **state)
{
	node_ptr test =  (node_ptr)malloc(sizeof(node));
	test = NULL;
	uint32_t data;
	uint8_t index;

	//Checking whether the List is Empty
	data = 40;
	index = 3;
    	assert_int_equal(EMPTY_LIST, addNode(&test, data, index));

	//Passing null pointer
	node_ptr *test1 =  (node_ptr)malloc(sizeof(node));
	test1 = NULL;
	assert_int_equal(NULL_PTR, addNode(test1, data, index));

	//adding node to an empty list
	data = 10;
	index = 1;
    	assert_int_equal(SUCCESS, addNode(&test, data, index));

	//adding node in index 1 of list
	data = 20;
	index = 1;
    	assert_int_equal(SUCCESS, addNode(&test, data, index));
	
	//adding node to the end of list
	data = 30;
	index = 3;
    	assert_int_equal(SUCCESS, addNode(&test, data, index));

	//adding node in the middle of list
	data = 40;
	index = 3;
    	assert_int_equal(SUCCESS, addNode(&test, data, index));

	//adding node in the middle of list
	data = 50;
	index = 3;
    	assert_int_equal(SUCCESS, addNode(&test, data, index));

	//adding node with index greater than size
	data = 50;
	index = 10;
    	assert_int_equal(INDEX_EXCEED, addNode(&test, data, index));

}

static void test_removeNode(void **state)
{
	node_ptr test =  (node_ptr)malloc(sizeof(node));
	test = NULL;
	uint32_t *data =  (uint32_t *)malloc(sizeof(uint32_t));
	data = NULL;
	uint8_t index;
	uint32_t ret_val;

	//checking Null pointer of data
	assert_int_equal(NULL_PTR, removeNode(&test, data, index));
	
	//Passing null pointer of dll
	node_ptr *test1 =  (node_ptr)malloc(sizeof(node));
	test1 = NULL;
	assert_int_equal(NULL_PTR, addNode(test1, data, index));


	//adding data randomly to the list
	addNode(&test, 10, 1);
	addNode(&test, 20, 2);
	addNode(&test, 30, 3);
	addNode(&test, 40, 4);
	addNode(&test, 50, 5);
	data = &ret_val;
	//removing at the end of list
	index = 5;
	ret_val = 50;
	assert_int_equal(SUCCESS, removeNode(&test, data, index));
	//comparing node values before and after removing
	assert_int_equal(ret_val, *data);
	//removing at index greater than size
	assert_int_equal(INDEX_EXCEED, removeNode(&test, data, index));
	

	//adding data randomly to the list
	addNode(&test, 10, 1);
	addNode(&test, 20, 2);
	addNode(&test, 30, 3);
	addNode(&test, 40, 4);
	addNode(&test, 50, 5);
	
	//removing from middle of list
	index = 3;
	ret_val = 30;
	assert_int_equal(SUCCESS, removeNode(&test, data, index));
	//comparing node values before and after removing
	assert_int_equal(ret_val, *data);
	//removing at index greater than size

	//removing from start of list
	index = 1;
	ret_val = 10;
	assert_int_equal(SUCCESS, removeNode(&test, data, index));
	//comparing node values before and after removing
	assert_int_equal(ret_val, *data);
	//removing at index greater than size


}

static void test_searchNode(void **state)
{

	node_ptr test =  (node_ptr)malloc(sizeof(node));
	test = NULL;
	uint32_t data;
	uint8_t *index = (uint8_t *)malloc(sizeof(uint8_t));
	uint8_t position;
	//adding data randomly to the list
	addNode(&test, 10, 1);
	addNode(&test, 20, 2);
	addNode(&test, 30, 3);
	addNode(&test, 40, 4);
	addNode(&test, 50, 5);
	
	//searching for a data which returns index
	data = 30;
	position = 3;
	assert_int_equal(SUCCESS, search(test, data, index));
	//comparing index values
	assert_int_equal(position, *index);

	//searching for another data which returns index
	data = 50;
	position = 5;
	assert_int_equal(SUCCESS, search(test, data, index));
	//comparing index values
	assert_int_equal(position, *index);

	//searching for data which is not present in list
	data = 100;
	assert_int_equal(FAIL, search(test, data, index));

}

static void test_destroy(void **state)
{
	//check for null pointer
	node_ptr *test1 =  (node_ptr)malloc(sizeof(node));
	test1 = NULL;
	assert_int_equal(NULL_PTR, destroy(test1));
	
	//adding data randomly to the list
	node_ptr test =  (node_ptr)malloc(sizeof(node));
	test = NULL;
	addNode(&test, 10, 1);
	addNode(&test, 20, 2);
	addNode(&test, 30, 3);
	addNode(&test, 40, 4);
	addNode(&test, 50, 5);
	
	//destroying list
	assert_int_equal(SUCCESS, destroy(&test));
	
}

static void test_size(void **state)
{
	//adding random values to the list	
	node_ptr test =  (node_ptr)malloc(sizeof(node));
	test = NULL;
	addNode(&test, 10, 1);
	addNode(&test, 20, 2);
	addNode(&test, 30, 3);
	addNode(&test, 40, 4);
	addNode(&test, 50, 5);
	//checking size of the list
	int size = 5;
	assert_int_equal(size, findSize(test));
}

int main(void) 
{
    const UnitTest tests[] = 
    {
        unit_test(test_addNode),
	unit_test(test_removeNode),
	unit_test(test_searchNode),
	unit_test(test_size),
	unit_test(test_destroy),
    };
    return run_tests(tests);
}

