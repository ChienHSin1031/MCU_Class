#ifndef HY_SPI_H
#define HY_SPI_H

#include "stdint.h"


#define GPIO_SPI_MISO   7
#define GPIO_SPI_MOSI   17


#define GPIO_SPI_SCK    14


#define GPIO_FLASH_CS    13



void HY_FLASH_SPI_Write_Read(uint8_t *tx_buf, uint32_t tx_len,uint8_t *rx_buf, uint32_t rx_len);
void W25Q_Flash_Write(uint8_t *dat);
int HY_DeinitSPI(void);
void HY_W25Q_Flash_check(void);




#endif