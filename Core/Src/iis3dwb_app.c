/*
 * iis3dwb_app.c
 *
 *      Author: Severin
 */
/* Includes ------------------------------------------------------------------------- */
#include "iis3dwb_app.h"

/* Declares ------------------------------------------------------------------------- */
void static IIS3DWB_SET_CONFIG();
extern UART_HandleTypeDef huart2;

/* Functions ------------------------------------------------------------------------ */
//Function:	Set-Function for the configuration of the IIS3DWB-sensor
void static IIS3DWB_SET_CONFIG(){
	uint8_t rst;

	//init driver interface
	//neccessary for iis3dwb driver (iis3dwb_reg)
	stmdev_ctx_t dev_ctx;
	uint8_t reg0;

	dev_ctx.read_reg = IIS3DWB_READ_REG;
	dev_ctx.write_reg = IIS3DWB_WRITE_REG;
	dev_ctx.handle = &spi_iis3dwb;

	/*
		1. WhoAmI
		2. Reset
		3. Block-Data-Update
		4. FIFO-Mode
		5. skala einstellen
		6. accelerometer enable
	 */

	/*while(1){
		iis3dwb_device_id_get(&dev_ctx, &iis3dwb_deviceID);
		HAL_UART_Transmit(&huart1, &iis3dwb_deviceID, sizeof(iis3dwb_deviceID), HAL_MAX_DELAY);
		HAL_Delay(1000);
	}*/

	//Get device ID
	iis3dwb_device_id_get(&dev_ctx, &iis3dwb_deviceID);
	//reset software in ctrl3_c
	iis3dwb_reset_set(&dev_ctx, PROPERTY_ENABLE);
	//continue after reset is finished
	do{
		iis3dwb_reset_get(&dev_ctx, &rst);
	}while(rst);

    // 3. Enables accelerometer:IIS3DWB_CTRL1_XL bit 7:5 XL_EN[2:0]
    /*(000: Power-down (default);
    101: accelerometer enabled;)
    All other configurations are not allowed*/
    iis3dwb_read_reg(&dev_ctx, IIS3DWB_CTRL1_XL, (uint8_t *)&reg0, 1);
    reg0 |= 0xA0;	//1010 0000
    iis3dwb_write_reg(&dev_ctx, IIS3DWB_CTRL1_XL, (uint8_t *)&reg0, 1);

    // 4. IIS3DWB_CTRL4_C bit 2
    /* Disable I2C*/
    iis3dwb_i2c_interface_set(&dev_ctx, IIS3DWB_I2C_DISABLE);

    //5. Set fifo in continuous / stream mode 110
    //110: Continuous mode: if the FIFO is full, the new sample overwrites the older one;
    iis3dwb_fifo_mode_set(&dev_ctx, IIS3DWB_STREAM_MODE);

    //6. Set watermark 128
    //IIS3DWB_FIFO_CTRL1 7:0 IIS3DWB_FIFO_CTRL2 bit 0
    uint8_t IIS3DWB_WTM_LEVEL = 128;
    iis3dwb_fifo_watermark_set(&dev_ctx, IIS3DWB_WTM_LEVEL);

    //7. Data Ready pulse mode
    //COUNTER_BDR_REG1 (0Bh) bit 7 dataready_pulsed
    //0: Data-ready latched mode
    //1: Data-ready pulsed mode (the data ready pulses are 18.75 �s long)
    iis3dwb_data_ready_mode_set(&dev_ctx, IIS3DWB_DRDY_PULSED);

    //8. Set full scale
    /* CTRL1_XL (10h) bit 3:2  00 (default) �2 g  01 �16 g 10 �4 g 11 �8 g*/
    // set scale to +- 2g
    iis3dwb_xl_full_scale_set(&dev_ctx, IIS3DWB_2g);

    /* 9. Set 2nd stage filter  output from first stage digital filtering selected
    * CTRL1_XL (10h) bit 0
    * (0: output from first stage digital filtering selected (default);
    * 1: output from LPF2 second filtering stage selected)
    *  	IIS3DWB_LP_ODR_DIV_4              = 0x80,
    	IIS3DWB_LP_ODR_DIV_10             = 0x81,
    	IIS3DWB_LP_ODR_DIV_20             = 0x82,
    	IIS3DWB_LP_ODR_DIV_45             = 0x83,
    	IIS3DWB_LP_ODR_DIV_100            = 0x84,
    	IIS3DWB_LP_ODR_DIV_200            = 0x85,
    	IIS3DWB_LP_ODR_DIV_400            = 0x86,
    	IIS3DWB_LP_ODR_DIV_800            = 0x87,
    **/
    iis3dwb_xl_hp_path_on_out_set(&dev_ctx,IIS3DWB_LP_ODR_DIV_20);

    // 10. FIFO_WTM_IA routing on pin INT1
    /* INT1_CTRL (0Dh) bit 3 Set to 1
     * INT1_FIFO_TH Enables FIFO threshold interrupt on INT1 pin.
     *
     * MD1_CFG (5Eh) Functions routing on INT1 register (r/w) disabled
     *  */
    iis3dwb_pin_int1_route_t pin_int1_route;
    *(uint8_t*)&(pin_int1_route.int1_ctrl) = 0;
    *(uint8_t*)&(pin_int1_route.md1_cfg) = 0;
    pin_int1_route.int1_ctrl.int1_fifo_th = 1;
    iis3dwb_pin_int1_route_set(&dev_ctx, &pin_int1_route);

    //11. Enable writing to FIFO
    /* FIFO_CTRL3 (09h) bit 3:0 Selects Batch Data Rate (write frequency in FIFO) for accelerometer data.
     * (0000: Accelerometer not batched in FIFO (default);
     * 1010: 26667 Hz;
     * */
    iis3dwb_fifo_xl_batch_set(&dev_ctx, IIS3DWB_XL_BATCHED_AT_26k7Hz);

	iis3dwb_xl_axis_selection_set(&dev_ctx, IIS3DWB_ENABLE_ALL);
	//enable block data update
	iis3dwb_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
	//set output data rate - enable acc
	iis3dwb_xl_data_rate_set(&dev_ctx, IIS3DWB_XL_ODR_26k7Hz);

	//enable auto increment
	iis3dwb_auto_increment_set(&dev_ctx, PROPERTY_ENABLE);
	//enable fifo bypass mode
	iis3dwb_fifo_mode_set(&dev_ctx,IIS3DWB_BYPASS_MODE);
	//use LP filter
	iis3dwb_xl_filter_lp2_set(&dev_ctx, PROPERTY_ENABLE);
	//enable all axis
	iis3dwb_xl_axis_selection_set(&dev_ctx,IIS3DWB_ENABLE_ALL);
}

void IIS3DWB_MEASSURE(){
	int16_t tmp_1[3] = { 0 };
	int16_t tmp_2 = 0;

	char msg_0[10] = {'\0'};
	msg_0[9] = '\n';
	char msg_1[10] = {'\0'};
	msg_1[9] = '\n';
	char msg_2[10] = {'\0'};
	msg_2[9] = '\n';

	//iis3dwb_config();
	//init driver interface
	//neccessary for iis3dwb driver (iis3dwb_reg)
	stmdev_ctx_t dev_ctx;
	dev_ctx.read_reg = IIS3DWB_READ_REG;
	dev_ctx.write_reg = IIS3DWB_WRITE_REG;
	dev_ctx.handle = &spi_iis3dwb;

	//get data ready flag
	iis3dwb_xl_flag_data_ready_get(&dev_ctx, &iis3dwb_dataReady);

	//check if acceleration data is ready -> read data
	if(iis3dwb_dataReady == 1){
		memset(data_raw_acceleration, 0x00, 6 * sizeof(uint8_t));
		iis3dwb_acceleration_raw_get(&dev_ctx, data_raw_acceleration);

		tmp_2 = (data_raw_acceleration[1] << 8) | data_raw_acceleration[0];
		tmp_1[0] = tmp_2;
		tmp_2 = (data_raw_acceleration[3] << 8) | data_raw_acceleration[2];
		tmp_1[1] = tmp_2;
		tmp_2 = (data_raw_acceleration[5] << 8) | data_raw_acceleration[4];
		tmp_1[2] = tmp_2;

		//convert data to engineering units
		acceleration_mg[0] = iis3dwb_from_fs2g_to_mg(tmp_1[0]);
		acceleration_mg[1] = iis3dwb_from_fs2g_to_mg(tmp_1[1]);
		acceleration_mg[2] = iis3dwb_from_fs2g_to_mg(tmp_1[2]);

		ftoa(acceleration_mg[0], msg_0, 2);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg_0, sizeof(msg_0), HAL_MAX_DELAY);
		ftoa(acceleration_mg[1], msg_1, 2);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg_1, sizeof(msg_1), HAL_MAX_DELAY);
		ftoa(acceleration_mg[2], msg_2, 2);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg_2, sizeof(msg_2), HAL_MAX_DELAY);
	}
}

//Function:	Init-Function for the IIS3DWB-sensor
//para_1:	HAL SPI handler name from STM32 (Pointer)
//para_2:	HAL GPIO-bank-handler name from STM32 for CS-signal (Pointer)
//para_3:	HAL GPIO-PIN name from STM32 for CS-signal
void IIS3DWB_INIT(SPI_HandleTypeDef spi, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
	spi_iis3dwb = spi;	//Setting the SPI handler for the IIS3DWB-sensor
	cs_gpiox_iis3dwb = GPIOx;	//Setting the GPIO-handler for the IIS3DWB-sensor for the relevant PIN-bank
	cs_gpio_iis3dwb = GPIO_PIN;	//Setting the GPIO-PIN for the low-active chip select for the IIS3DWB-sensor

	IIS3DWB_CS_HIGH();			//Setting the cs-signal high (LOW-ACTIVE!) for the default state; only set to low for transmitting or receiving
//	IIS3DWB_3_4_SEL_HIGH();
	IIS3DWB_SET_CONFIG();
}
