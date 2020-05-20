
#ifdef _cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

	
#include "HY_Type.h"
#include "HY_SPI.h"
#include "nrf_drv_spi.h"

	

#define HW_SPI_ID nrf_drv_spi_t

typedef struct spi_device_config {
        HW_SPI_ID bus_id; /**< SPI id as needed by hw_spi_... functions */
        nrf_drv_spi_config_t hw_init;
        bool ignore_cs; /**< true if in slave mode CS line should be ignored */
} spi_device_config;	

#define SPI_SLAVE_DEVICE(name, cs_port, cs_pin, _word_mode, spi_mode, spi_rate, bit_order_t) \
				        const spi_device_config dev_##name = { \
								.bus_id = NRF_DRV_SPI_INSTANCE(2) ,\
								.hw_init.frequency = spi_rate,\
								.hw_init.irq_priority = SPI_DEFAULT_CONFIG_IRQ_PRIORITY,\
								.hw_init.miso_pin = GPIO_SPI_MISO,\
								.hw_init.mode = spi_mode,\
								.hw_init.mosi_pin = GPIO_SPI_MOSI,\
								.hw_init.sck_pin = GPIO_SPI_SCK,\
								.hw_init.ss_pin = cs_pin,\
								.hw_init.bit_order = bit_order_t, \
								.hw_init.orc       = 0x5A,\
        }; \
				const void *const name = &dev_##name;	

				

//SPI_SLAVE_DEVICE(ACC, 0, 	GPIO_ACC_CS, 0, NRF_DRV_SPI_MODE_0, NRF_DRV_SPI_FREQ_4M, NRF_DRV_SPI_BIT_ORDER_MSB_FIRST)	
SPI_SLAVE_DEVICE(FLASH, 0,	GPIO_FLASH_CS, 0, NRF_DRV_SPI_MODE_0, NRF_DRV_SPI_FREQ_4M, NRF_DRV_SPI_BIT_ORDER_MSB_FIRST)

static spi_device_config *cur_dev_cfg = NULL;
				
typedef void* spi_dev_handle_t;

int HY_initSPI(void)
{

		
	return RLT_SUCCESS;
}



static void HY_WriteDataSPI(spi_device_config *t,uint8_t *dat, uint32_t length) 
{
	
	if (t != cur_dev_cfg)
	{
		cur_dev_cfg = t;
		
		nrf_drv_spi_uninit(&(cur_dev_cfg->bus_id));
		
		nrf_drv_spi_init(&(cur_dev_cfg->bus_id),&(cur_dev_cfg->hw_init),NULL,NULL);
		
	}
	

	nrf_drv_spi_transfer(&(cur_dev_cfg->bus_id), dat, length, NULL, 0);
	
	
}

static void HY_Write_ReadDataSPI(spi_device_config *t,uint8_t *tx_buf, uint32_t tx_len,uint8_t *rx_buf, uint32_t rx_len) 
{
	 uint32_t flags = 0;
	
	if (t != cur_dev_cfg)
	{
		
		cur_dev_cfg = t;
		
		nrf_drv_spi_uninit(&(cur_dev_cfg->bus_id));
		
		nrf_drv_spi_init(&(cur_dev_cfg->bus_id),&(cur_dev_cfg->hw_init),NULL,NULL);
		
	}

	
	nrf_drv_spi_transfer(&(cur_dev_cfg->bus_id), tx_buf, tx_len, rx_buf, rx_len);
					
	
}
/*
void HY_LCD_SPI_Write(uint8_t *dat, uint32_t length) 
{
	//HY_WriteDataSPI((spi_device_config *)COLOR_LCD,dat,length);
}

void HY_ACC_SPI_Write_Read(uint8_t *tx_buf, uint32_t tx_len,uint8_t *rx_buf, uint32_t rx_len) 
{
	HY_Write_ReadDataSPI((spi_device_config *)ACC,tx_buf,tx_len,rx_buf,rx_len);
}

void HY_FLASH_SPI_Write_Read(uint8_t *tx_buf, uint32_t tx_len,uint8_t *rx_buf, uint32_t rx_len) 
{
	HY_Write_ReadDataSPI((spi_device_config *)FLASH,tx_buf,tx_len,rx_buf,rx_len);
}
*/
int HY_DeinitSPI(void)
{
    if (cur_dev_cfg != NULL)
        nrf_drv_spi_uninit(&(cur_dev_cfg->bus_id));

	return RLT_SUCCESS;
}

#ifdef _cplusplus // if compiled with C++ compiler
} // end of extern "C" block
#endif
