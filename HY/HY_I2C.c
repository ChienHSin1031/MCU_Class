/*****************************************************************************
 *   HY_InitSys.c:  system initialization
 *
 *   Copyright(C) 2020, HY Technology, Inc.
 *   All rights reserved.
 *
 *   ECR List:
 *
******************************************************************************/

#ifdef _cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app_twi.h"
#include "app_error.h"

#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "HY_I2C.h"

#define HW_I2C_ID nrf_drv_twi_t	

	
typedef struct i2c_device_config {
        HW_I2C_ID bus_id; /**< SPI id as needed by hw_spi_... functions */
        nrf_drv_twi_config_t hw_init;
} i2c_device_config;		
	
#define I2C_SLAVE_DEVICE(name,i2c_scl,i2c_sda) \
        const i2c_device_config dev_##name = { \
              .bus_id = NRF_DRV_TWI_INSTANCE(0) ,\
              .hw_init.scl = i2c_scl,\
              .hw_init.sda = i2c_sda,\
              .hw_init.frequency = NRF_TWI_FREQ_400K,\
              .hw_init.interrupt_priority = APP_IRQ_PRIORITY_LOW,\
              }; \
const void *const name = &dev_##name;		

	
I2C_SLAVE_DEVICE(I2C0,GPIO_TWI0_CLK,GPIO_TWI0_DATA)		
I2C_SLAVE_DEVICE(I2C1,GPIO_TWI1_CLK,GPIO_TWI1_DATA)					

static i2c_device_config *cur_dev_cfg = NULL;	
		
static bool bEnable = false;				

static bool RLT_SUCCESS = true;

int HY_initI2C(void)
{
	uint32_t err_code;

	err_code = nrf_drv_twi_init(&((i2c_device_config *)I2C0)->bus_id, &((i2c_device_config *)I2C0)->hw_init, NULL, NULL);	

	APP_ERROR_CHECK(err_code);

	nrf_drv_twi_enable(&((i2c_device_config *)I2C0)->bus_id);
	
	cur_dev_cfg = (i2c_device_config *)I2C0;
	
	bEnable = true;

	return RLT_SUCCESS;
}

void HY_I2C_Enable(bool enable)
{
	bEnable = enable;
	
	if (enable)
	{
          
	}else
	{
		
        nrf_drv_twi_disable(&(cur_dev_cfg)->bus_id);

		if (cur_dev_cfg != NULL)
			nrf_drv_twi_uninit(&(cur_dev_cfg)->bus_id);
		
		cur_dev_cfg = NULL;
		
	}	
	
}

int HY_I2C0_read_reg(uint8_t address,uint8_t cmdID, uint8_t bytes, uint8_t *pRegVal)
{
	uint8_t send_buf[16] = {0};
	int i = 0;
	
	
	if (cur_dev_cfg != I2C0)
	{
			uint32_t err_code;
		
			if (cur_dev_cfg != NULL)
				
                        nrf_drv_twi_uninit(&(cur_dev_cfg)->bus_id);
		
			err_code = nrf_drv_twi_init(&((i2c_device_config *)I2C0)->bus_id, &((i2c_device_config *)I2C0)->hw_init, NULL, NULL);	
		
			nrf_drv_twi_enable(&((i2c_device_config *)I2C0)->bus_id);
		
			cur_dev_cfg = (i2c_device_config *)I2C0;
	}
	
	send_buf[i++] = cmdID;
	
	nrf_drv_twi_xfer_desc_t xfer_des = {
					.type = NRF_DRV_TWI_XFER_TXRX,///< Type of transfer.
					.address = address,           ///< Slave address.
					.primary_length = i,          ///< Number of bytes transferred.
					.secondary_length = bytes,    ///< Number of bytes transferred.
					.p_primary_buf = send_buf,    ///< Pointer to transferred data.
					.p_secondary_buf = pRegVal,   ///< Pointer to transferred data.
	};	
	
					
	int result = nrf_drv_twi_xfer(&((i2c_device_config *)I2C0)->bus_id, &xfer_des, 0);
	
	return 0; 
	
}

int HY_I2C0_write_reg(uint8_t address, uint8_t cmdID, uint8_t regVal)
{
	uint8_t send_buf[16] = {0};
	
	if (cur_dev_cfg != I2C0)
	{
			uint32_t err_code;
		
			if (cur_dev_cfg != NULL)
                            nrf_drv_twi_uninit(&(cur_dev_cfg)->bus_id);
		
			err_code = nrf_drv_twi_init(&((i2c_device_config *)I2C0)->bus_id, &((i2c_device_config *)I2C0)->hw_init, NULL, NULL);	
		
			nrf_drv_twi_enable(&((i2c_device_config *)I2C0)->bus_id);
		
			cur_dev_cfg = (i2c_device_config *)I2C0;
	}	
	
	int i = 0;
	send_buf[i++] = cmdID;
	send_buf[i++] = regVal;	
	
	nrf_drv_twi_xfer_desc_t xfer_des = {
					.type = NRF_DRV_TWI_XFER_TX,             ///< Type of transfer.
					.address = address,          ///< Slave address.
					.primary_length = i,   ///< Number of bytes transferred.
					.p_primary_buf = send_buf,    ///< Pointer to transferred data.
	};
	
  int result = nrf_drv_twi_xfer(&((i2c_device_config *)I2C0)->bus_id, &xfer_des, 0);
	
	return 0;
}


int HY_I2C1_read_reg(uint8_t address,uint8_t cmdID, uint8_t bytes, uint8_t *pRegVal)
{
	uint8_t send_buf[16] = {0};
	int i = 0;
	
	
	if (cur_dev_cfg != I2C1)
	{
			uint32_t err_code;
		
			if (cur_dev_cfg != NULL)
				nrf_drv_twi_uninit(&(cur_dev_cfg)->bus_id);
		
			err_code = nrf_drv_twi_init(&((i2c_device_config *)I2C1)->bus_id, &((i2c_device_config *)I2C1)->hw_init, NULL, NULL);	
		
			nrf_drv_twi_enable(&((i2c_device_config *)I2C1)->bus_id);
		
			cur_dev_cfg = (i2c_device_config *)I2C1;
	}
	
	send_buf[i++] = cmdID;
	
	nrf_drv_twi_xfer_desc_t xfer_des = {
					.type = NRF_DRV_TWI_XFER_TXRX,///< Type of transfer.
					.address = address,          ///< Slave address.
					.primary_length = i,        ///< Number of bytes transferred.
					.secondary_length = bytes,   ///< Number of bytes transferred.
					.p_primary_buf = send_buf,    ///< Pointer to transferred data.
					.p_secondary_buf = pRegVal,  ///< Pointer to transferred data.
	};	
	
					
	int result = nrf_drv_twi_xfer(&((i2c_device_config *)I2C1)->bus_id, &xfer_des, 0);
	
	return 0; 
	
}

int HY_I2C1_write_reg(uint8_t address,uint8_t cmdID, uint8_t regVal)
{
	uint8_t send_buf[16] = {0};
	
	
	if (cur_dev_cfg != I2C1)
	{
			uint32_t err_code;
		
			if (cur_dev_cfg != NULL)
                            nrf_drv_twi_uninit(&(cur_dev_cfg)->bus_id);
		
			err_code = nrf_drv_twi_init(&((i2c_device_config *)I2C1)->bus_id, &((i2c_device_config *)I2C1)->hw_init, NULL, NULL);	
		
			nrf_drv_twi_enable(&((i2c_device_config *)I2C1)->bus_id);
		
			cur_dev_cfg = (i2c_device_config *)I2C1;
	}	
	
	int i = 0;
	send_buf[i++] = cmdID;
	send_buf[i++] = regVal;	
	
	nrf_drv_twi_xfer_desc_t xfer_des = {
					.type = NRF_DRV_TWI_XFER_TX,             ///< Type of transfer.
					.address = address,          ///< Slave address.
					.primary_length = i,   ///< Number of bytes transferred.
					.p_primary_buf = send_buf,    ///< Pointer to transferred data.
	};
	
	
  int result = nrf_drv_twi_xfer(&((i2c_device_config *)I2C1)->bus_id, &xfer_des, 0);
	
	return 0;
}


int HY_DeinitI2C(void)
{

	if (cur_dev_cfg != NULL)
		nrf_drv_twi_uninit(&(cur_dev_cfg)->bus_id); 

	return RLT_SUCCESS;
}

#ifdef _cplusplus // if compiled with C++ compiler
} // end of extern "C" block
#endif
