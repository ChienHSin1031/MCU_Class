# MCU_Class

## 4/8 
1. 只留 main code, init及其他副程式移到別的檔案

main.c
```
#include "HY_BLE.h"
- original code
int main(void){

    ble_init();
    // Enter main loop.
    for (;;)
    {
        ble_hy_idele_state_handle();
    }
}
```
HY_ble.c
```
+ original code 
void ble_init(void)
{
    bool erase_bonds;

    // Initialize.
    log_init();
    timers_init();
    buttons_leds_init(&erase_bonds);
    power_management_init();
    ble_stack_init();
    gap_params_init();
    gatt_init();
    advertising_init();
    services_init();
    sensor_simulator_init();
    conn_params_init();
    peer_manager_init();
    // Start execution.
    NRF_LOG_INFO("Heart Rate Sensor example started.");
    
    application_timers_start();
    advertising_start(erase_bonds);
}
```

HY_ble.h  //header file  format 
```
#ifndef HY_BLE_H
#define HY_BLE_H

void ble_init(void);
void ble_hy_idele_state_handle(void);

#endif
```

2. Timeout 不要讓他3分鐘就近睡眠 
HY_ble.c
```
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    ret_code_t err_code;
    bool erase_bonds;
    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            NRF_LOG_INFO("Fast advertising.");
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_ADV_EVT_IDLE:
-           //sleep_mode_enter();            // don't run sleep_mode
+           advertising_start(erase_bonds);  //Rebroadcast the ble
            break;

        default:
            break;
    }
}


void bsp_event_handler(bsp_event_t event)
{
    ret_code_t err_code;
    bool erase_bonds;

    switch (event)
    {
        case BSP_EVENT_SLEEP:
-           //sleep_mode_enter();             // don't run sleep_mode
+           advertising_start(erase_bonds);  //Rebroadcast the ble
            break;

        case BSP_EVENT_DISCONNECT:
            err_code = sd_ble_gap_disconnect(m_conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
            {
                err_code = ble_advertising_restart_without_whitelist(&m_advertising);
                if (err_code != NRF_ERROR_INVALID_STATE)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;

        default:
            break;
    }
}

```

## 4/20 add the new app_timer 

HY_ble.h
```
#ifndef HY_BLE_H
#define HY_BLE_H

void ble_init(void);
void ble_hy_idele_state_handle(void);

+ void HY_TIMER_init(void);
+ void HY_timeout_handler(void * p_context);
#endif
```

HY_TIMER.c  (all new)
```
#include "app_timer.h"
#include "nrf_log.h"

APP_TIMER_DEF(m_HY_timer_id);       //definition the app_timer id  

void HY_TIMER_init(void);            //don't forget the definition the function 
void HY_timers_start(void);
void hy_timeout_handler(void * p_context);

#define HY_INTERVAL         APP_TIMER_TICKS(1000) //set app_timer to one second interval              

void HY_TIMER_init(void){       //timer init
  //Init Timer module.
  ret_code_t err_code; 
  APP_ERROR_CHECK(err_code);

  // create Timer.
  err_code = app_timer_create(&m_HY_timer_id,     
                              APP_TIMER_MODE_REPEATED,
                              hy_timeout_handler);
  APP_ERROR_CHECK(err_code);  

  HY_timers_start();         //go to start timer
}

void HY_timers_start(void){
    ret_code_t err_code;

    // Start application timers.
    err_code = app_timer_start(m_HY_timer_id, HY_INTERVAL, NULL);     //start the timer and set the interval
    APP_ERROR_CHECK(err_code);

}

void hy_timeout_handler(void * p_context){      //when timer trigger run code 

    NRF_LOG_INFO("this HY timer,one second/s");    
    UNUSED_PARAMETER(p_context);

    }
```
main.c 
```
/**
 * Copyright (c) 2014 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @example examples/ble_peripheral/ble_app_hrs/main.c
 *
 * @brief Heart Rate Service Sample Application main file.
 *
 * This file contains the source code for a sample application using the Heart Rate service
 * (and also Battery and Device Information services). This application uses the
 * @ref srvlib_conn_params module.
 */

original code 

  #include "HY_BLE.h"
+ #include "HY_TIMER.h"

#include "nrf_drv_clock.h"
int main(void){

      ble_init();
+     HY_TIMER_init();
    
    //git test
    // Enter main loop.
    for (;;)
    {
        ble_hy_idele_state_handle();
    }
}

```


