/*****************************************************************************
	* Copyright (C) 2017 by Sai Raghavendra Sankrantipati
	*
	* Redistribution, modification or use of this software in source or binary
	* forms is permitted as long as the files maintain this copyright. Users are
	* permitted to modify this and use it to learn about the field of embedded
	* software. Sai Raghavendra Sankrantipati and the University of Colorado are not liable for
	* any misuse of this material.
	
*****************************************************************************/ 
/**
 * @file dll.h
 * @A Doubly Linked List implementation
 * @author Sai Raghavendra Sankrantipati
 * @date Septempber 8 2017
 */


#ifndef _DLL_H_
#define _DLL_H_

#include<stdint.h>


/** Enum for function returns**/
typedef enum
{
	SUCCESS,
	EMPTY_LIST,
	INDEX_EXCEED,
	NULL_PTR,
	FAIL,    
} dll_return;

typedef struct node *node_ptr;
/**structure of a Node in the Linked List**/
typedef struct node
{
	node_ptr next;
	node_ptr prev;
	uint32_t data;
}node;


dll_return addNode(node_ptr *head, uint32_t data, uint8_t index);
dll_return destroy(node_ptr *head);
dll_return search(node_ptr head, uint32_t data, uint8_t * index);
dll_return removeNode(node_ptr *head, uint32_t *data, uint8_t  index);
uint8_t findSize(node_ptr head);
dll_return printList(node_ptr head);



#endif

