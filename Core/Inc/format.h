/*
 * format.h
 *
 *  Created on: Sep 23, 2021
 *      Author: Severin
 */

#ifndef INC_FORMAT_H_
#define INC_FORMAT_H_

/* Includes ------------------------------------------------------------------------- */
#include "math.h"
#include "stdio.h"

/* Functions ------------------------------------------------------------------------ */
void reverse(char* str, int len);
int itostr(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);
uint8_t* u32tou8(uint32_t n);

#endif /* INC_FORMAT_H_ */
