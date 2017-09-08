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
 
#ifndef DLL_H
#define DLL_H
#include<stdint.h>
#include<stdbool.h>

Struct Node
{
	uint8_t index;
	uint32_t data;
	Struct Node *prev;
	Struct Node *next;
};

uint8_t addNode(Struct Node **head, uint32_t data, uint8_t index);
uint8_t removeNode(Struct **head, uint32_t *data, uint8_t index);
uint8_t destroy(Struct Node **head);
void Search(Struct **head, uint32_t *data, uint8_t *index);
void size(Struct Node **head, uint8_t *size);

 #endif