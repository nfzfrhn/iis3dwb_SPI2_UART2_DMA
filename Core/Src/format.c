/*
 * format.c
 *
 *  Created on: Sep 23, 2021
 *      Author: Severin
 */

/* Includes ------------------------------------------------------------------------- */
#include "format.h"

//Function:	Reverses a string
//para_1:	String to be reversed
//para_2:	Length of the string to be reversed
//return:	void
void reverse(char* str, int len){
	int i = 0;
	int j = len -1;
	int tmp;
	while(i < j){
		tmp = str[i];										//Store the i-element of string in tmp
		str[i] = str[j];									//Store the j-element of the string in the i-element of the string
		str[j] = tmp;										//Store tmp in the j-element of the string
		i++;
		j--;
	}
}

//Function:	Converts an integer to string
//para_1:	Integer to be converted
//para_2:	String which the integer is converted to
//para_3:	Number of digits required in output
//return:	void
int itostr(int x, char str[], int d){
	int i = 0;
	while(x){
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}
	while(i < d){
		str[i++] = '0';
	}
	reverse(str, i);
	str[i] = '\0';
	return i;
}

//Function:	Converts a float to a string
//para_1:	Float to be converted
//para_2:	String which the float is converted to
//para_3:	Number of digits behind the point
//return:	void
void ftoa(float n, char* res, int afterpoint){
	int ipart = (int)n;										//Integer-part of float n stored in ipart
	float fpart = n - (float)ipart;							//Float-part of float n stored in fpart
	int i = itostr(ipart, res, 0);							//Convert the integer-part to string
	if(afterpoint != 0){
		res[i] = '.';
		fpart = fpart * pow(10, afterpoint);
		itostr((int)fpart, (res + i + 1), afterpoint);
	}
}

uint8_t* u32tou8(uint32_t n){
	static uint8_t res[4];
	res[0] = (0x000000FF & n);
	res[1] = ((0x0000FF00 & n) >> 8);
	res[2] = ((0x00FF0000 & n) >> 16);
	res[3] = ((0xFF000000 & n) >> 24);
	return res;
}
