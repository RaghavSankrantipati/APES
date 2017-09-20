/**************************************************************************************
*@Filename:dll.c
*
*@Description: Implementation of Doubly linked list
* Operations included are adding node, removing node, destroying list,
*finding size of list, searching list and printing elements
*@Author:Sai Raghavendra Sankrantipati
*@Date:19/07/2017
*@compiler:gcc
**************************************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include "dll.h"


/**This functin adds node and checks 5 different conditions. 
*a. Check for Null pointer
*b. check for empty list 
*c. check for index during empty list
*d. adding to an uninitialised list
*e. adding at start, middle and end of a list **/

dll_return addNode(node_ptr *head, uint32_t data, uint8_t index)
{

	if(head == NULL)
	    return NULL_PTR;

	if(*head==NULL && index == 1)
	{
		node_ptr temp = (node_ptr)malloc(sizeof(node));
		temp->data = data;
		*head = temp;
		return SUCCESS;
	}

	if(*head == NULL && index!=1)
		return EMPTY_LIST;

	if((*head != NULL) && (index ==1))
	{   
	    node_ptr temp = (node_ptr)malloc(sizeof(node));
		temp->data = data;
		temp->next = *head;
		temp->prev = NULL;
		(*head)->prev = temp;
		*head = temp;
		return SUCCESS;
	}

	if(*head!=NULL && index !=1)
	{
		node_ptr temp;
		temp = *head;
		uint8_t size;
		size = 	findSize(temp);

		node_ptr new_node = (node_ptr)malloc(sizeof(node));
		
		if(size == (index-1))
		{
			while(temp->next != NULL)
			{
				temp = temp->next;
			}
			
			new_node->data = data;
			new_node->next = NULL;
			new_node->prev = temp;
			temp->next = new_node;
			
			return SUCCESS;
			
		}
		
		if(size<index)
			return INDEX_EXCEED;
		
		if(size>(index-1))
		{
			for(int m=0; m<(index-2); m++)
			{
				temp = temp->next;
			}
			new_node->data = data;
			new_node->prev = temp;
			new_node->next = temp->next;
			(temp->next)->prev = new_node;
			temp->next = new_node;
			
			return SUCCESS;
		}
	}
	return FAIL;
}


/** This function destroys list and returns success and if Null pointer 
* is passed it returns error**/
dll_return destroy(node_ptr *head)
{
    if(head == NULL)
	return NULL_PTR;
	
    node_ptr temp;
    temp = *head;
    node_ptr temp1 = temp;

    while(temp!=NULL){

        temp1 = temp;
        temp = temp->next;
        free(temp1);
    }
	
	free(temp);
	*head = NULL;
	return SUCCESS;
	
}

/**This functions search for a given data and returns an error**/
dll_return search(node_ptr head, uint32_t data, uint8_t * index)
{
    node_ptr temp;
    temp = head;
   
    uint8_t count = 1;
   
    if(temp!=NULL)
    {
           uint8_t size;
        size =     findSize(temp);
        for(int i=0; i<size; i++)
        {
            if(temp->data == data)
                return SUCCESS;
            temp = temp->next;
            count++;
            *index = count;
        }
        return FAIL;
    }
   
    if(temp == NULL)
    {
        return FAIL;

    }
   
}


/**This functin removes node and checks 3 different conditions. 
*a. Check for Null pointer
*b. check for empty list 
*c. removing at start, middle and end of a list **/	
dll_return removeNode(node_ptr *head, uint32_t *data, uint8_t index)
{
	if(data==NULL)
		return NULL_PTR;
	if(head == NULL)
		return EMPTY_LIST;
	
	node_ptr temp = *head;
	uint8_t size;
	size  = findSize(temp);

	if(size < index)
		return INDEX_EXCEED;

	if(size == index)
	{
	    for(int i=0; i<(index-1); i++)
        {
	        temp=temp->next;
        }
	    *data=temp->data;
		temp->prev->next=NULL;
		
		free(temp);
		return SUCCESS;
	}
	
	if(index==1)
	{
	    *data = temp->data;
		temp->next->prev = NULL;
		*head = temp->next;
		free(temp);
		return SUCCESS;

	}

	if(size>index && index!=1)
	{
	    for(int i=0; i<(index-1); i++)
        {
	        temp=temp->next;
        }
		(temp->prev)->next = temp->next;
		(temp->next)->prev = temp->prev;
		*data = temp->data;
		free(temp);
		return SUCCESS;
	}

}

/** This functions returns the size of a list**/

uint8_t findSize(node_ptr head)
{
	node_ptr temp = head;
    uint8_t count = 0;
 
	while(temp != NULL)
	{
		count++;
		if(temp->next == NULL){
		    return count;
		}
		temp=temp->next;
	}

	return count;
}

/**IT dumps all the elements in the list**/
dll_return printList(node_ptr head)
{    
    printf("\n PRINTING LIST :");
    if(head == NULL)
    {
        printf("\nEMPTY LIST");
        return SUCCESS;
    }
	node_ptr temp;
	temp = head;
	
	while(temp != NULL)
	{
		printf("\n%d", temp->data);
		if(temp->next == NULL)
		    return SUCCESS;
		temp=temp->next;
	}
		free(temp);
	return SUCCESS;
}
