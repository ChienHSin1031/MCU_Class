#include <stdio.h>
#include "app_error.h"
#include "Message.h"
#include "nrf_log.h"
#include "nrf_drv_twi.h"

#define TWI_SCL_M           5       //!< Master SCL pin.
#define TWI_SDA_M           6       //!< Master SDA pin.



#define CW6301_Write_PS          (0x12 >> 1)  //write modr address
#define CW6301_Read_PS          (0x13 >> 1)  //read modr address

#define CW6301_OUT1                 0x0A
#define OUT1_Vol                 (0x80|0x37)  //2.7mV

#define CW6301_OUT2                 0x0B
#define OUT2_Vol                 (0x80|0x3C)  //3.2mV

#define CW6301_OUT3                 0x0C
#define OUT3_Vol                 (0x80|0x1C)  //1.8mV

#define CW6301_OUT4                 0x0D
#define OUT4_Vol                 (0x80|0x1C)  //1.8mV



#define CW6301_System_Check         0x00

static uint8_t m_sample;

static volatile bool m_xfer_done = false;

static const nrf_drv_twi_t m_twi_master = NRF_DRV_TWI_INSTANCE(0);

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);


/*
void ret_code_t HY_twi_master_init(void){
    twi_master_init();
}
*/
/*					
void read_sensor_data()
{
    NRF_LOG_INFO("go read_sensor_data");
    /* Read 1 byte from the specified address - skip 3 bits dedicated for fractional part of temperature. */
//    ret_code_t err_code = nrf_drv_twi_rx(&m_twi_master, CW6301_Read_PS, &m_sample, sizeof(m_sample));
    //data_handler(m_sample);
/*
    NRF_LOG_INFO("0x%x", m_sample);
    while (m_xfer_done == false);  
    APP_ERROR_CHECK(err_code);
}

*/
__STATIC_INLINE void data_handler(uint8_t System)
{
    NRF_LOG_INFO("Temperature: 0x%x Celsius degrees.",System);
}



ret_code_t twi_master_init(void)
{
    ret_code_t ret;

    const nrf_drv_twi_config_t config =
    {
       .scl                = TWI_SCL_M,
       .sda                = TWI_SDA_M,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
       .clear_bus_init     = false
    };

    ret = nrf_drv_twi_init(&m_twi_master, &config, twi_handler, NULL); //twi_handler

    if (NRF_SUCCESS == ret)
    {
        nrf_drv_twi_enable(&m_twi_master);	
    }

    return ret;
}

void HY_Power_Supply_Set(){
    
  ret_code_t err_code;

  uint8_t reg[7]= {CW6301_OUT1, OUT1_Vol,CW6301_OUT2, OUT2_Vol, CW6301_OUT3, OUT3_Vol, CW6301_OUT4, OUT4_Vol};
  //uint8_t reg[0]= CW6301_System_Check;
  err_code = nrf_drv_twi_tx(&m_twi_master, CW6301_Write_PS, reg, sizeof(reg), false);
  APP_ERROR_CHECK(err_code);
  nrf_drv_twi_enable(&m_twi_master);
  while (m_xfer_done == false);
  NRF_LOG_INFO("go nrf_drv_twi_tx");
  
}




void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    NRF_LOG_INFO("go twi_handler");

    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
                {
                    data_handler(m_sample);
                    NRF_LOG_INFO("go data_handler");
                }   
            else if(p_event->xfer_desc.type == NRF_DRV_TWI_XFER_TX)
                {
                    NRF_LOG_INFO("twi tx done");
                }

            m_xfer_done = true;

            break;
        
        default:
            break;
    }
}