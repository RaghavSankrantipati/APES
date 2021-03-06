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
 * @file circbuff.h
 * @A circular buffer implementation
 * @author Sai Raghavendra Sankrantipati
 * @date Septempber 8 2017
 */
 
#ifndef CIRCBUFF_H
#define CIRCBUFF_H
#include<stdint.h>
#include<stdbool.h>
#include<string.h>
 
 struct CircBuff
 {
	 uint32_t *head;
	 uint32_t *tail;
	 uint8_t size;
	 uint8_t count;
	 uint32_t *base;
	 uint32_t *end;
 };
 
 uint8_t allocate(struct CircBuff **base, uint8_t size);
 void destroy(struct CircBuff *buff);
 uint8_t isFull(struct CircBuff *buff);
 uint8_t isEmpty(struct CircBuff *buff);
 uint8_t addItem(struct CircBuff *buff, uint32_t *data);
 void dump(struct CircBuff *buff);
 uint8_t size(struct CircBuff *buff);
 
 #endif
 
