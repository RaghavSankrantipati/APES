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
 * @file dll.c
 * @A Doubly Linked List implementation
 * @author Sai Raghavendra Sankrantipati
 * @date Septempber 8 2017
 */
 
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include "dll.h"

uint8_t addNode(struct Node **head, uint32_t *data, uint8_t index)
{
	struct Node *temp;
	temp = *head;
	while(temp->index!=index)
	{
		temp = temp->next;
	}
	struct Node *new = (struct Node *)malloc(sizeof(struct Node));  		    //dynamic memory allocation for a node
	new->next = temp->next;														//create a new node and change next and previous 
	new->data = *data;															// node address for new node, prev and next node
	new->prev = temp->prev;													
	temp->next = new;
	(new->next)->prev = new;
	temp = new;
	while(temp->next!=NULL)
	{
		temp=temp->next;
		temp->index++;
	}
	return 0;
}

uint8_t removeNode(struct Node **head, uint32_t *data, uint8_t index)
{
	struct Node *temp;
	temp = *head;
	while(temp->index!=index)
	{
		temp = temp->next;
	}
	(temp->prev)->next = temp->next;										//change next and previous address of next and previous nodes
	(temp->next)->prev = temp->prev;
	*data = temp->data;
	
	struct Node *temp1;
	temp1 = temp->next;
	free(temp);
	
	while(temp1->next != NULL)
	{
		temp1->index--;
		temp=temp->next;
	}
}

uint8_t destroy(struct Node **head)
{
	struct Node *temp1, *temp;
	temp1 = *head;
	while(temp->next!=NULL)
	{
		temp=temp->next;
		free(temp->prev);
	}
	free(temp);
	
	return 1;
}

void Search(struct Node **head, uint32_t *data, uint8_t *index)
{
	struct Node *temp;
	temp = *head;
	while(temp->data != *data)
	{
		temp=temp->next;
	}
	
	*index =  temp->index;
}

void size(struct Node **head, uint8_t *size)
{
	struct Node *temp;
	temp = *head;
	while(temp->next!=NULL)
	{
		temp = temp->next;
	}
	*size = temp->index;
}
