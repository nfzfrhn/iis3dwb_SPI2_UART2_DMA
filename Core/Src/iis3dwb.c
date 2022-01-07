/*
 * iis3dwb.c
 *
 *      Author: Severin
 */

/* Includes ------------------------------------------------------------------------- */
#include "iis3dwb.h"

/* Functions ------------------------------------------------------------------------ */

//Function:	Writing a generic register
//para_1:	Bus-handler; spi_iis3dwb (Pointer)
//para_2:	Register address/target
//para_3:	To be transmitted data
//para_4:	Length of the to be transmitted data in Byte
//return:	State of IIS3DWB
int32_t IIS3DWB_READ_REG(void *handle, uint8_t address, uint8_t *rx_data, uint16_t length){
	address = address | 0x80;	//Set Read Bit in Address
	IIS3DWB_CS_LOW();	//Setting the CS-signal active = LOW
	//Read Register
	if(HAL_OK != HAL_SPI_Transmit(handle, &address, 1, HAL_MAX_DELAY)){
		return IIS3DWB_ERROR;
	}
	if(HAL_OK != HAL_SPI_Receive(handle, rx_data, length, HAL_MAX_DELAY)){
		return IIS3DWB_ERROR;
	}
	HAL_Delay(100);
	IIS3DWB_CS_HIGH();	//Setting the CS-signal deactive = HIGH
	return IIS3DWB_OK;
}

//Function:	Writing a generic register
//para_1:	Bus-handler; spi_iis3dwb (Pointer)
//para_2:	Register address/target
//para_3:	To be transmitted data
//para_4:	Length of the to be transmitted data in Byte
//return:	State of IIS3DWB
int32_t IIS3DWB_WRITE_REG(void *handle, uint8_t address, uint8_t *tx_data, uint16_t length){
	IIS3DWB_CS_LOW();	//Setting the CS-signal active = LOW
	//Transmitting the register address
	if(HAL_OK != HAL_SPI_Transmit(handle, &address, 1, IIS3DWB_SPI_TIMEOUT)){
		return IIS3DWB_ERROR;
	}
	//Transmitting the data
	if(HAL_OK != HAL_SPI_Transmit(handle, tx_data, length, IIS3DWB_SPI_TIMEOUT)){
		return IIS3DWB_ERROR;
	}
	IIS3DWB_CS_HIGH();	//Setting the CS-signal deactive = HIGH
	return IIS3DWB_OK;
}

//Function:	Sets the GPIO-PIN responsible for the CS-signal to HIGH (LOW-ACTIVE!)
void IIS3DWB_CS_HIGH(){
	HAL_GPIO_WritePin(cs_gpiox_iis3dwb, cs_gpio_iis3dwb, GPIO_PIN_SET);
}

//Function:	Sets the GPIO-PIN responsible for the CS-signal to HIGH (LOW-ACTIVE!)
//void IIS3DWB_3_4_SEL_HIGH(){
//	HAL_GPIO_WritePin(SEL_3_4_GPIO_Port, SEL_3_4_Pin, GPIO_PIN_SET);
//}

//Function:	Sets the GPIO-PIN responsible for the CS-signal to LOW (LOW-ACTIVE!)
void IIS3DWB_CS_LOW(){
	HAL_GPIO_WritePin(cs_gpiox_iis3dwb, cs_gpio_iis3dwb, GPIO_PIN_RESET);
}

