/*
 * iis3dwb_app.h
 *
 *      Author: Severin
 */

#ifndef INC_IIS3DWB_APP_H_
#define INC_IIS3DWB_APP_H_

#include "iis3dwb.h"
#include "iis3dwb_st.h"
#include "iis3dwb_reg.h"
#include "format.h"
//#include "main.h"

#define IIS3DWB_SAMPLES_PER_IT  (128)

/* Variables ------------------------------------------------------------------------ */
//ToDo: Reduce global variables
SPI_HandleTypeDef spi_iis3dwb;
GPIO_TypeDef *cs_gpiox_iis3dwb;
uint16_t cs_gpio_iis3dwb;

/* Functions ------------------------------------------------------------------------ */
//void static IIS3DWB_SET_CONFIG();

uint8_t iis3dwb_deviceID;
uint8_t iis3dwb_dataReady;

uint8_t data_raw_acceleration[6];
float acceleration_mg[3];

void IIS3DWB_INIT(SPI_HandleTypeDef spi, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN);
void IIS3DWB_MEASSURE();
void IIS3DWB_Int_Measure();
void checkStatus();

#endif /* INC_IIS3DWB_APP_H_ */
