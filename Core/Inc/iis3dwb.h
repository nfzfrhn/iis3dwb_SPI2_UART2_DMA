/*
 * iis3dwb.h
 *
 *      Author: Severin
 */

#ifndef INC_IIS3DWB_H_
#define INC_IIS3DWB_H_

/* Includes ------------------------------------------------------------------------- */
#include "main.h"
#include "iis3dwb_app.h"

/* Defines -------------------------------------------------------------------------- */
#define IIS3DWB_SPI_TIMEOUT 1000

/* Declares ------------------------------------------------------------------------- */
/* Enumerations & Structures -------------------------------------------------------- */


/* Variables ------------------------------------------------------------------------ */

/* Functions ------------------------------------------------------------------------ */
int32_t IIS3DWB_READ_REG(void *handle, uint8_t address, uint8_t *rx_data, uint16_t length);
int32_t IIS3DWB_WRITE_REG(void *handle, uint8_t address, uint8_t *tx_data, uint16_t length);

void IIS3DWB_CS_HIGH();					//CS is low active so IIS3DWB_CS_HIGH() = 0 and IIS3DWB_CS_LOW() = 1
void IIS3DWB_CS_LOW();

void IIS3DWB_3_4_SEL_HIGH();			//Pull the SEL_3_4 high for the SPI Clock

#endif /* INC_IIS3DWB_H_ */
