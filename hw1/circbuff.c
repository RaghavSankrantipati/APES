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
 * @file circbuff.c
 * @A circular buffer implementation
 * @author Sai Raghavendra Sankrantipati
 * @date Septempber 8 2017
 */
 
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include "circbuff.h"

 
 uint8_t allocate (struct CircBuff **base, uint8_t size)
 {
	 struct CircBuff *buff;
	 buff->base = (uint32_t *)malloc(sizeof(uint32_t)*size);
	 *base = buff;
	 buff->head = buff->base;
	 buff->tail = buff->head;
	 buff->size = size;
	 buff->count = 0;
	 return 1;
 }
 
void destroy(struct CircBuff *buff)
{
	free(buff);
}

uint8_t isFull(struct CircBuff *buff)
{
	if(buff->count == buff->size) return 1;
	else return 0;
}

uint8_t isEmpty(struct CircBuff *buff)
{
	if(buff->count == 0) return 1;
	else return 0;
}

uint8_t addItem(struct CircBuff *buff, uint32_t *data)
{
	if(isFull(buff)==1) 
		return 0;
	else
	{
		if(buff->head==buff->end)
			buff->head=buff->base;
		else
			buff->head=buff->head + sizeof(uint32_t);  				//move head to the next index
			
		memcpy(buff->head, data, sizeof(uint32_t));								//move data from data pointer to buffer head
		buff->count++;
		
		return 1;
	}
}

uint8_t removeItem(struct CircBuff *buff, uint32_t *data)
{
	if(isEmpty(buff)==1)
	{
		return 0;
	}
	else 
	{
		if(buff->tail == buff->end)
			buff->tail = buff->base;
		else 
			buff->tail = buff->tail + sizeof(uint32_t);						//move tail to the next index
		
		memcpy(data, buff->tail, sizeof(uint32_t));									//move data from buffer tail to data pointer
		buff->count--;
		return 1;
	}
}

void dump(struct CircBuff *buff)
{
	uint32_t *temp;
	temp = buff->head;
	while(temp != buff->tail + sizeof(uint32_t))
	{
		printf("\n %d", *temp);
		temp = temp + sizeof(uint32_t);
	}
}

uint8_t size(struct CircBuff *buff)
{
	return buff->size;
}
